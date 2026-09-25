LATEX = latex
DVIPS = dvips
DVIPDF = dvipdf
BIBTEX = bibtex

EPSFIGS = nine-two-sum.eps normal_sum_prod.eps qd_add.eps qd_add_proof.eps \
          qd_add_qd_d.eps qd_mul_accum.eps qd_mul_qd_d.eps quick-two-sum.eps \
          six-three-sum.eps three-sum-2.eps three-sum-3.eps three-sum.eps \
          two-prod.eps two-sum.eps 

all: qd.pdf td.pdf

dvi: qd.dvi td.dvi

pdf: qd.pdf td.pdf

ps: qd.ps td.ps

qd.dvi: qd.tex qd.bib $(EPSFIGS)
	$(LATEX) $< && $(BIBTEX) qd && \
	$(LATEX) $< && $(LATEX) $<

qd.ps: qd.dvi
	$(DVIPS) -o $@ $<

qd.pdf: qd.dvi
	$(DVIPDF) $<

td.dvi: td.tex
	$(LATEX) $< && $(LATEX) $<

td.ps: td.dvi
	$(DVIPS) -o $@ $<

td.pdf: td.dvi
	$(DVIPDF) $<

clean:
	rm -f qd.dvi qd.blg qd.bbl qd.toc qd.log qd.aux
	rm -f td.dvi td.log td.aux td.out

maintainer-clean: clean
	rm -f qd.ps qd.pdf td.ps td.pdf

.PHONY: clean maintainer-clean all
