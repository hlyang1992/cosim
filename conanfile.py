import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import load


class helloRecipe(ConanFile):
    name = "cosim"

    package_type = "application"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_tests": [True, False],
        "build_examples": [True, False],
        "build_cli": [True, False],
        "build_clib": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "build_tests": False,
        "build_examples": False,
        "build_cli": True,
        "build_clib": True,
    }

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "*"


    def requirements(self):
        self.tool_requires("thrift/[~0.13]")
        self.requires("fmt/[10.2.1]") 
        self.requires("thrift/[~0.13]")
        self.requires("spdlog/[~1.14]")
        self.requires("libzip/[~1.10]") 
        self.requires("cli11/[~2.4]")
        self.requires("fmilibrary/[~2.3]")
        self.requires("pugixml/1.14")
        if self.options.build_tests:
            self.requires("catch2/[~3.6]")
  
    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
        self.options["*"].shared = self.options.shared

    def set_version(self):
        self.version = load(self, os.path.join(self.recipe_folder, "version.txt")).strip()

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)

        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.cache_variables["ECOS_BUILD_TESTS"] = self.options.build_tests
        tc.variables["ECOS_BUILD_EXAMPLES"] = self.options.build_examples
        tc.variables["ECOS_BUILD_CLI"] = self.options.build_cli
        tc.variables["ECOS_BUILD_CLIB"] = self.options.build_clib
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    # def package_info(self):
    #     self.cpp_info.libs = ["cosim"]
    #     self.cpp_info.set_property("cmake_target_name", "libcosim::cosim")

    
