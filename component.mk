#
# Component Makefile
#

# From src/Makefile.am:
cxx98_sources = \
	compatibility.cc \
	compatibility-debug_list.cc \
	compatibility-debug_list-2.cc

cxx11_sources = \
	compatibility-c++0x.cc \
	compatibility-atomic-c++0x.cc \
	compatibility-thread-c++0x.cc \
	compatibility-chrono.cc \
	compatibility-condvar.cc

# From src/c++11/Makefile.am:
cxx11_host_sources = \
	ctype_configure_char.cc \
	ctype_members.cc

# ENABLE_DUAL_ABI=1
cxx11_abi_sources = \
	cow-locale_init.cc \
	cow-shim_facets.cc \
	cxx11-hash_tr1.cc \
	cxx11-ios_failure.cc \
	cxx11-shim_facets.cc \
	cxx11-stdexcept.cc

cxx11_cxx11_sources = \
	chrono.cc \
	codecvt.cc \
	condition_variable.cc \
	cow-stdexcept.cc \
	ctype.cc \
	debug.cc \
	functexcept.cc \
	functional.cc \
	futex.cc \
	future.cc \
	hash_c++0x.cc \
	hashtable_c++0x.cc \
	ios.cc \
	limits.cc \
	mutex.cc \
	placeholders.cc \
	random.cc \
	regex.cc  \
	shared_ptr.cc \
	snprintf_lite.cc \
	system_error.cc \
	thread.cc \
	${cxx11_abi_sources} \
	${cxx11_host_sources}

# ENABLE_DUAL_ABI=1
cxx11_extra_string_inst_sources = \
	cow-fstream-inst.cc \
	cow-sstream-inst.cc \
	cow-string-inst.cc \
	cow-wstring-inst.cc \
	cxx11-locale-inst.cc \
	cxx11-wlocale-inst.cc

# ENABLE_EXTERN_TEMPLATE=1
cxx11_inst_sources = \
	$(cxx11_extra_string_inst_sources) \
	ext11-inst.cc \
	fstream-inst.cc \
	ios-inst.cc \
	iostream-inst.cc \
	istream-inst.cc \
	locale-inst.cc \
	ostream-inst.cc \
	sstream-inst.cc \
	streambuf-inst.cc \
	string-inst.cc \
	wlocale-inst.cc \
	wstring-inst.cc

# From src/c++98/Makefile.am:
# ENABLE_DUAL_ABI=1
cxx98_cow_string_host_sources = \
	collate_members_cow.cc \
	messages_members_cow.cc \
	monetary_members_cow.cc \
	numeric_members_cow.cc

# Source files linked in via configuration/make substitution for a
# particular host.
cxx98_host_sources = \
	$(cxx98_cow_string_host_sources) \
	atomicity.cc \
	codecvt_members.cc \
	collate_members.cc \
	messages_members.cc \
	monetary_members.cc \
	numeric_members.cc \
	time_members.cc

# ENABLE_DUAL_ABI=1
cxx98_cxx11_abi_sources = \
	cow-istream-string.cc

# ENABLE_EXTERN_TEMPLATE=1
cxx98_inst_sources = \
	allocator-inst.cc \
	concept-inst.cc \
	ext-inst.cc \
	misc-inst.cc

cxx98_parallel_sources = parallel_settings.cc

cxx98_host_sources_extra = \
	basic_file.cc c++locale.cc \
	${cxx98_inst_sources} ${cxx98_parallel_sources}

# Sources present in the src directory, always present.
cxx98_cxx98_sources = \
	bitmap_allocator.cc \
	pool_allocator.cc \
	mt_allocator.cc \
	codecvt.cc \
	complex_io.cc \
	globals_io.cc \
	hash_tr1.cc \
	hashtable_tr1.cc \
	ios_failure.cc \
	ios_init.cc \
	ios_locale.cc \
	list.cc \
	list-aux.cc \
	list-aux-2.cc \
	list_associated.cc \
	list_associated-2.cc \
	locale.cc \
	locale_init.cc \
	locale_facets.cc \
	localename.cc \
	math_stubs_float.cc \
	math_stubs_long_double.cc \
	stdexcept.cc \
	strstream.cc \
	tree.cc \
	istream.cc \
	istream-string.cc \
	streambuf.cc \
	valarray.cc \
	${cxx98_cxx11_abi_sources} \
	${cxx98_host_sources} \
	${cxx98_host_sources_extra}

libsupcxx_sources = \
	array_type_info.cc \
	atexit_arm.cc \
	atexit_thread.cc \
	bad_alloc.cc \
	bad_array_length.cc \
	bad_array_new.cc \
	bad_cast.cc \
	bad_typeid.cc \
	class_type_info.cc \
	del_op.cc \
	del_ops.cc \
	del_opnt.cc \
	del_opv.cc \
	del_opvs.cc \
	del_opvnt.cc \
	dyncast.cc \
	eh_alloc.cc \
	eh_arm.cc \
	eh_aux_runtime.cc \
	eh_call.cc \
	eh_catch.cc \
	eh_exception.cc \
	eh_globals.cc \
	eh_personality.cc \
	eh_ptr.cc \
	eh_term_handler.cc \
	eh_terminate.cc \
	eh_tm.cc \
	eh_throw.cc \
	eh_type.cc \
	eh_unex_handler.cc \
	enum_type_info.cc \
	function_type_info.cc \
	fundamental_type_info.cc \
	guard.cc \
	guard_error.cc \
	hash_bytes.cc \
	nested_exception.cc \
	new_handler.cc \
	new_op.cc \
	new_opnt.cc \
	new_opv.cc \
	new_opvnt.cc \
	pbase_type_info.cc \
	pmem_type_info.cc \
	pointer_type_info.cc \
	pure.cc \
	si_class_type_info.cc \
	tinfo.cc \
	tinfo2.cc \
	vec.cc \
	vmi_class_type_info.cc \
	vterminate.cc

#	libstdc++-v3/include/std \
#	libstdc++-v3/include/c \
#
#
#	libstdc++-v3/include/c_global \
#	libstdc++-v3/include/std \
#
COMPONENT_ADD_INCLUDEDIRS := \
	libstdc++-v3/include \
	libstdc++-v3/libsupc++ \
	libstdc++-v3/include/c_global \
	libstdc++-v3/include/std \
	libgcc \
	include \
	include/xtensa-esp32-elf \
	.

COMPONENT_SRCDIRS := \
	libstdc++-v3/libsupc++ \
	libstdc++-v3/src/c++11 \
	libstdc++-v3/src/c++98 \
	libstdc++-v3/src \
	.

COMPONENT_OBJS := \
	$(cxx98_sources:%.cc=libstdc++-v3/src/c++98/%.o) \
	$(cxx11_sources:%.cc=libstdc++-v3/src/c++11/%.o) \
	\
	$(cxx11_cxx11_sources:%.cc=libstdc++-v3/src/c++11/%.o) \
	$(cxx11_inst_sources:%.cc=libstdc++-v3/src/c++11/%.o) \
	\
	$(cxx98_cxx98_sources:%.cc=libstdc++-v3/src/c++98/%.o) \
	\
	$(libsupcxx_sources:%.cc=libstdc++-v3/libsupc++/%.o)

# Remove default -std=c++11 / -std=gnu++11
CXXFLAGS:=$(filter-out -std=c++11,$(filter-out -std=gnu++11,$(CXXFLAGS)))

# Fix for ESP32 PSRAM issue, apply to all files
CFLAGS += -mfix-esp32-psram-cache-issue
CXXFLAGS += -nostdinc++ -mfix-esp32-psram-cache-issue

## Fix warnings
#libstdc++-v3/libsupc++/del_ops.o: CXXFLAGS += -Wno-c++14-compat
#libstdc++-v3/libsupc++/del_opvs.o: CXXFLAGS += -Wno-c++14-compat

# Use special rules for C++11 files/objects.
libstdc++-v3/src/c++11/compatibility-c++0x.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/compatibility-atomic-c++0x.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/compatibility-thread-c++0x.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/compatibility-chrono.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/compatibility-condvar.o: CXXFLAGS += -std=gnu++11

# src/c++11/Makefile.am
libstdc++-v3/src/c++11/chrono.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/codecvt.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/condition_variable.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cow-fstream-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cow-locale_init.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cow-shim_facets.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cow-sstream-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cow-stdexcept.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cow-string-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cow-wstring-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/ctype.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cxx11-hash_tr1.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cxx11-ios_failure.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cxx11-locale-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cxx11-shim_facets.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cxx11-stdexcept.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/cxx11-wlocale-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/debug.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/ext11-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/fstream-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/functexcept.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/functional.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/futex.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/future.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/hash_c++0x.o: CXXFLAGS += -std=gnu++11
#libstdc++-v3/src/c++11/hashtable_c++0x.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/ios-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/ios.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/iostream-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/istream-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/limits.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/locale-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/mutex.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/ostream-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/placeholders.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/random.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/regex.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/shared_ptr.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/snprintf_lite.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/sstream-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/streambuf-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/string-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/system_error.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/thread.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/wlocale-inst.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++11/wstring-inst.o: CXXFLAGS += -std=gnu++11
# Use special rules for the hashtable.cc file so that all
# the generated template functions are also instantiated. 
libstdc++-v3/src/c++11/hashtable_c++0x.o: CXXFLAGS += -std=gnu++11 -fimplicit-templates

# src/c++98/Makefile.am
# Use special rules to compile with the non-default string ABI.
libstdc++-v3/src/c++98/collate_members_cow.o: CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0 -fimplicit-templates
libstdc++-v3/src/c++98/messages_members_cow.o: CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0 -fimplicit-templates
libstdc++-v3/src/c++98/monetary_members_cow.o: CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0 -fimplicit-templates
libstdc++-v3/src/c++98/numeric_members_cow.o: CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0 -fimplicit-templates
libstdc++-v3/src/c++98/locale_init.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++98/localename.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/src/c++98/strstream.o: CXXFLAGS += -I$(COMPONENT_PATH)/libstdc++-v3/include/backward -Wno-deprecated
libstdc++-v3/src/c++98/concept-inst.o: CXXFLAGS += -D_GLIBCXX_CONCEPT_CHECKS -fimplicit-templates
libstdc++-v3/src/c++98/parallel_settings.o: CXXFLAGS += -D_GLIBCXX_PARALLEL

# libsupc++/Makefile.am
libstdc++-v3/libsupc++/cp-demangle.o: CXXFLAGS += -DIN_GLIBCPP_V3 -Wno-error
# Use special rules for the C++11 sources so that the proper flags are passed.
libstdc++-v3/libsupc++/bad_array_length.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/bad_array_new.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/eh_aux_runtime.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/eh_ptr.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/eh_terminate.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/eh_throw.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/guard.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/atexit_thread.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/nested_exception.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/new_handler.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/new_op.o: CXXFLAGS += -std=gnu++11
libstdc++-v3/libsupc++/new_opnt.o: CXXFLAGS += -std=gnu++11

# Use special rules for the C++14 sources so that the proper flags are passed.
libstdc++-v3/libsupc++/del_ops.o: CXXFLAGS += -std=gnu++14 -Wno-sized-deallocation
libstdc++-v3/libsupc++/del_opvs.o: CXXFLAGS += -std=gnu++14 -Wno-sized-deallocation

libstdc++-v3/src/c++11/ctype_members.o: CXXFLAGS += -Wno-type-limits
