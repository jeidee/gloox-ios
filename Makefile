PROJ := libgloox

GLOOX_VER := 1.0.16
GLOOX_NAME := gloox-$(GLOOX_VER)
GLOOX_TAR := $(GLOOX_NAME).tar.bz2

CC := $(shell xcrun --sdk iphoneos -f clang)
SDKROOT := $(shell xcrun --sdk iphoneos --show-sdk-path)
ARCH := arm64

SRCDIR := ./$(GLOOX_NAME)/src
BUILDDIR := ./build
TARGETDIR := ./libgloox
TARGET := $(TARGETDIR)/lib/$(PROJ).a

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -maxdepth 1 -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

CFLAGS := -arch $(ARCH) -isysroot $(SDKROOT) -miphoneos-version-min=7.0 -fembed-bitcode
CFLAGS += -I$(GLOOX_NAME)/src

all: add_sources $(TARGET)

gloox:
	test -f $(GLOOX_TAR) || wget http://camaya.net/download/$(GLOOX_TAR)
	test -d $(GLOOX_NAME) || tar xvfz $(GLOOX_TAR)

add_sources:
	cp ./src/* $(SRCDIR)/

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR)/lib
	@mkdir -p $(TARGETDIR)/include
	@echo "Build shared library..."
	ar rcs $@ $(BUILDDIR)/*.o
	ranlib $@
	cp -f ./src/gloox_interface.h ./src/gloox_callback.h $(TARGETDIR)/include/


$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "Compile ...$<"
	@mkdir -p $(BUILDDIR)
	cp -f ./config.h.ios $(SRCDIR)/config.h.unix
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(TARGETDIR)

distclean:
	rm -rf $(BUILDDIR)
	rm -rf $(TARGETDIR)
	rm -rf $(GLOOX_NAME)
