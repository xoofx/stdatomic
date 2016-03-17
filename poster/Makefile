define lu-define-flavor-XELATEX
  $$(eval $$(call lu-create-flavor,XELATEX,tex,XELATEX,.pdf,pdf,\
    .pdftex_t .$$(_LU_XELATEX_EXT)))
endef

PATH:=./pygments:$(PATH) 
LU_MASTERS=poster
LU_FLAVORS=XELATEX
XELATEX=xelatex -shell-escape

include /usr/include/LaTeX.mk

megaclean:
	git clean -dfx

