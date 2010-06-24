all:
	cd build && cmake . && make -j4
depends:
	cd .. && git submodule init && git submodule update
	case "`uname`" in \
		*arwin*) \
			cd ../externals/macdepends && make \
		;; \
		*MINGW*|*CYGWIN*|*win32*) \
			echo WINDOWS \
		;; \
		*) \
			cd ../externals/linuxdepends && make \
		;; \
	esac
