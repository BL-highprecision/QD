#!/usr/bin/env bash
set -u

# Matrix test for QD with 16 configure combinations:
#   ieee_add   = yes/no
#   sloppy_mul = yes/no
#   sloppy_div = yes/no
#   fma        = yes/no  (--enable-fma=gnu -mfma / --disable-fma)
#
# Default usage:
#   bash misc/check_16_builds.sh
#
# Optional environment variables:
#   SRC_DIR=/path/to/QD
#   BUILD_ROOT=/path/to/build-root
#   JOBS=8
#   KEEP_BUILD=1   # keep existing build dirs instead of deleting them
#
# Notes:
# - This script expects an Autotools project with out-of-tree builds.
# - If the source tree was previously configured in-tree, this script will
#   try to clean it once before running the matrix.

SRC_DIR="${SRC_DIR:-$(pwd)}"
BUILD_ROOT="${BUILD_ROOT:-$SRC_DIR/_build_matrix}"
LOG_DIR="$BUILD_ROOT/logs"
MAKE_CMD="${MAKE:-make}"
KEEP_BUILD="${KEEP_BUILD:-0}"

detect_jobs() {
    if [ -n "${JOBS:-}" ]; then
        echo "$JOBS"
        return
    fi
    if command -v nproc >/dev/null 2>&1; then
        nproc
        return
    fi
    if command -v getconf >/dev/null 2>&1; then
        getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4
        return
    fi
    echo 4
}

JOBS="$(detect_jobs)"

enable_flag() {
    key="$1"
    val="$2"
    case "$key" in
        ieee_add)
            [ "$val" = "yes" ] && echo "--enable-ieee-add" || echo "--disable-ieee-add"
            ;;
        sloppy_mul)
            [ "$val" = "yes" ] && echo "--enable-sloppy-mul" || echo "--disable-sloppy-mul"
            ;;
        sloppy_div)
            [ "$val" = "yes" ] && echo "--enable-sloppy-div" || echo "--disable-sloppy-div"
            ;;
        fma)
            [ "$val" = "yes" ] && echo "--with-arch=x86-64-v3" || echo "--disable-fma"
            ;;
        *)
            echo "unknown configure flag key: $key" >&2
            exit 2
            ;;
    esac
}

prepare_source_tree() {
    echo "=== Preparing source tree ==="
    echo "SRC_DIR: $SRC_DIR"

    if [ ! -d "$SRC_DIR" ]; then
        echo "ERROR: source directory does not exist: $SRC_DIR" >&2
        exit 2
    fi

    if [ ! -x "$SRC_DIR/configure" ]; then
        echo "ERROR: configure script not found or not executable: $SRC_DIR/configure" >&2
        exit 2
    fi

    # If the source tree was configured in-tree before, out-of-tree configure
    # will fail. Clean it once here.
    if [ -f "$SRC_DIR/config.status" ]; then
        echo "Source tree appears to be configured in-tree."
        echo "Running one-time cleanup in source tree..."

        (
            set -e
            cd "$SRC_DIR"

            if [ -f Makefile ]; then
                "$MAKE_CMD" distclean || true
            fi

            # In some broken states, distclean may not remove these.
            rm -f config.status config.cache config.log
        ) || {
            echo "ERROR: failed to clean source tree: $SRC_DIR" >&2
            exit 2
        }
    fi

    # If config.status still exists, abort clearly.
    if [ -f "$SRC_DIR/config.status" ]; then
        echo "ERROR: source tree is still configured after cleanup." >&2
        echo "Please inspect and clean manually:" >&2
        echo "  cd \"$SRC_DIR\" && make distclean" >&2
        exit 2
    fi

    echo "Source tree is ready for out-of-tree builds."
    echo
}

mkdir -p "$BUILD_ROOT" "$LOG_DIR"

SUMMARY_OK="$BUILD_ROOT/summary.ok"
SUMMARY_NG="$BUILD_ROOT/summary.ng"
SUMMARY_TSV="$BUILD_ROOT/summary.tsv"
: > "$SUMMARY_OK"
: > "$SUMMARY_NG"
printf "tag\tresult\tlog\n" > "$SUMMARY_TSV"

fail_count=0

run_one() {
    ieee_add="$1"
    sloppy_mul="$2"
    sloppy_div="$3"
    fma="$4"

    tag="ieee_add-${ieee_add}__sloppy_mul-${sloppy_mul}__sloppy_div-${sloppy_div}__fma-${fma}"
    build_dir="$BUILD_ROOT/$tag"
    log_file="$LOG_DIR/$tag.log"

    if [ "$KEEP_BUILD" != "1" ]; then
        rm -rf "$build_dir"
    fi
    mkdir -p "$build_dir"

    echo "=== $tag ==="
    echo "build_dir: $build_dir"
    echo "log_file : $log_file"

    # CXXFLAGS: add -mfma only when fma=yes
    if [ "$fma" = "yes" ]; then
        cxxflags="-mfma"
    else
        cxxflags=""
    fi

    (
        set -e
        cd "$build_dir"

        "$SRC_DIR/configure" \
            --srcdir="$SRC_DIR" \
            "$(enable_flag ieee_add "$ieee_add")" \
            "$(enable_flag sloppy_mul "$sloppy_mul")" \
            "$(enable_flag sloppy_div "$sloppy_div")" \
            "$(enable_flag fma "$fma")" \
            CXXFLAGS="$cxxflags -O2"

        "$MAKE_CMD" -j"$JOBS"
        "$MAKE_CMD" check
        "$MAKE_CMD" -C tests huge
        ./tests/huge
    ) >"$log_file" 2>&1

    rc=$?
    if [ "$rc" -eq 0 ]; then
        echo "PASS $tag"
        echo "$tag" >> "$SUMMARY_OK"
        echo "--- log: $log_file ---"
        sed -n '/Testing dd_real \.\.\./,$p' "$log_file" || true
        printf "%s\tPASS\t%s\n" "$tag" "$log_file" >> "$SUMMARY_TSV"
    else
        echo "FAIL $tag"
        echo "$tag" >> "$SUMMARY_NG"
        printf "%s\tFAIL\t%s\n" "$tag" "$log_file" >> "$SUMMARY_TSV"
        echo "--- log: $log_file ---"
        sed -n '/Testing dd_real \.\.\./,$p' "$log_file" || true
        echo "---------------------------"
        fail_count=$((fail_count + 1))
    fi
    echo
}

run_default() {
    tag="default"
    build_dir="$BUILD_ROOT/$tag"
    log_file="$LOG_DIR/$tag.log"

    if [ "$KEEP_BUILD" != "1" ]; then
        rm -rf "$build_dir"
    fi
    mkdir -p "$build_dir"

    echo "=== $tag ==="
    echo "build_dir: $build_dir"
    echo "log_file : $log_file"

    (
        set -e
        cd "$build_dir"

        "$SRC_DIR/configure" --srcdir="$SRC_DIR"

        "$MAKE_CMD" -j"$JOBS"
        "$MAKE_CMD" check
        "$MAKE_CMD" -C tests huge
        ./tests/huge
    ) >"$log_file" 2>&1

    rc=$?
    if [ "$rc" -eq 0 ]; then
        echo "PASS $tag"
        echo "$tag" >> "$SUMMARY_OK"
        echo "--- log: $log_file ---"
        sed -n '/Testing dd_real \.\.\.$/,$p' "$log_file" || true
        printf "%s\tPASS\t%s\n" "$tag" "$log_file" >> "$SUMMARY_TSV"
    else
        echo "FAIL $tag"
        echo "$tag" >> "$SUMMARY_NG"
        printf "%s\tFAIL\t%s\n" "$tag" "$log_file" >> "$SUMMARY_TSV"
        echo "--- log: $log_file ---"
        sed -n '/Testing dd_real \.\.\.$/,$p' "$log_file" || true
        echo "---------------------------"
        fail_count=$((fail_count + 1))
    fi
    echo
}

prepare_source_tree

run_default

for ieee_add in no yes; do
    for sloppy_mul in no yes; do
        for sloppy_div in no yes; do
            for fma in no yes; do
                run_one "$ieee_add" "$sloppy_mul" "$sloppy_div" "$fma"
            done
        done
    done
done

echo "========================================"
echo "Finished."
echo "PASS list : $SUMMARY_OK"
echo "FAIL list : $SUMMARY_NG"
echo "TSV       : $SUMMARY_TSV"
echo "Logs      : $LOG_DIR"
echo "Failures  : $fail_count"

if [ "$fail_count" -ne 0 ]; then
    exit 1
fi
exit 0
