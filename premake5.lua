workspace "sandbox"
	configurations { "Debug", "Release" }

	filter "configurations:Debug"
		defines {"DEBUG"}

	filter "configurations:Release"
		defines {"NDEBUG"}

project "sb_main"
	kind "ConsoleApp"
	language "C"
	location "build"
	targetdir "build/out"
	includedirs {
		"sb_toolbelt"
	}
	files {
		"sb_main/*.c",
		"sb_main/*.h"
	}
	links {
		"sb_toolbelt"
	}

project "sb_toolbelt"
	kind "StaticLib"
	language "C"
	location "build"
	includedirs {
		"sb_toolbelt"
	}
	targetdir "build/out"
	files {
		"sb_toolbelt/*.c",
		"sb_toolbelt/*.h"
	}