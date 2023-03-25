workspace "Captain-Claw"
    configurations { "Debug", "Release" }
    architecture "x64"

project "Captain-Claw"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"

    output_dir = "%{cfg.buildcfg}-%{cfg.architecture}"
    targetdir("bin/%{prj.name}/" .. output_dir)
    objdir("intermediate/%{prj.name}/" .. output_dir)

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.cpp",
    }

    filter "configurations:*"
        defines { "SFML_STATIC" }
        includedirs { "vendor/sfml/include" }
        libdirs { "vendor/sfml/lib" }
        links { "opengl32.lib", "freetype.lib", "winmm.lib", "gdi32.lib", "openal32.lib", "flac.lib", "vorbisenc.lib", "vorbisfile.lib", "vorbis.lib", "ogg.lib", "ws2_32.lib" }

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        links { "sfml-audio-s-d.lib", "sfml-graphics-s-d.lib", "sfml-network-s-d.lib", "sfml-system-s-d.lib", "sfml-window-s-d.lib" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        links { "sfml-audio-s.lib", "sfml-graphics-s.lib", "sfml-network-s.lib", "sfml-system-s.lib", "sfml-window-s.lib" }
        optimize "On"