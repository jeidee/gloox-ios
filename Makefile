PROJ := libgloox

GLOOX_VER := 1.0.17
GLOOX_NAME := gloox-$(GLOOX_VER)
GLOOX_TAR := $(GLOOX_NAME).tar.bz2

CC := $(shell xcrun --sdk iphoneos -f clang)
SDKROOT := $(shell xcrun --sdk iphoneos --show-sdk-path)
ARCH := arm64

SRCDIR := ./$(GLOOX_NAME)/src
BUILDDIR := ./build
TARGETDIR := ./out/gloox
TARGET := $(TARGETDIR)/lib/$(PROJ).a

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -maxdepth 1 -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

CFLAGS := -arch $(ARCH) -isysroot $(SDKROOT) -miphoneos-version-min=7.0 -fembed-bitcode

all: get_gloox $(TARGET)

get_gloox:
	test -f $(GLOOX_TAR) || wget http://camaya.net/download/$(GLOOX_TAR)
	test -d $(GLOOX_NAME) || tar xvfz $(GLOOX_TAR)

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR)/lib
	@mkdir -p $(TARGETDIR)/include/gloox
	@echo "Build shared library... $<"
	ar rcs $@ $(BUILDDIR)/*.o
	ranlib $@
	mkdir -p $(TARGETDIR)/include
	cp -f $(SRCDIR)/*.h $(TARGETDIR)/include/gloox/


$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "Compile ...$<"
	@mkdir -p $(BUILDDIR)
	cp -f $(SRCDIR)/../config.h.unix.in $(SRCDIR)/config.h.unix
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(TARGETDIR)
	rm -rf $(GLOOX_TAR)
	rm -rf $(GLOOX_NAME)
