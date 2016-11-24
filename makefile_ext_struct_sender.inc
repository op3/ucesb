# -*- Makefile -*-

#########################################################

EXTWRITE = $(UCESB_BASE_DIR)/hbook/make_external_struct_sender.pl

EXTWRITE_GENDIR := $(BUILD_DIR)/gen

gen/extwrite_%.hh: $(EXTWRITE) #$(EXTWRITE_SOURCES)
	@echo "EXTWRSTR $@" # $(ASMGENFLAGS)
	@$(EXTWRITE) --struct=extwrite_$* < $(EXTWRITE_SOURCES) > $@ || rm $@

# This rule fixes up problems with auto-gen dependent paths
# in dependecy files.  Automatic dependencies are a pain.
#gen/extwrite_%.hh: $(EXTWRITE_GENDIR)/extwrite_%.hh
#	@echo " SPECIAL $@"

#########################################################