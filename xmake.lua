set_project("GAMES106-HW5")
set_version("0.0.1")

set_xmakever("2.7.4")

set_warnings("all")
set_languages("c++17")

add_rules("mode.debug", "mode.release", "mode.releasedbg")

set_runtimes("MD")
add_defines("NOMINMAX")
set_warnings("all")

add_requires("assimp", "glad")
add_requires("libigl", {configs={imgui = true}})

target("GAMES106-HW5")
    set_kind("binary")
    set_rundir("$(projectdir)")
    if is_mode("debug") then
            add_defines("DEBUG")
    end
    add_cxxflags("/bigobj")
    add_files("src/**.cpp")
    add_headerfiles("src/**.hpp")
    add_includedirs("src")
    add_packages(
        "glad",
        "libigl",
        "assimp")
target_end()