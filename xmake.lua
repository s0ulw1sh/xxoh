add_rules("mode.debug", "mode.release")

add_requires("zlib")

target("xxoh")
    set_kind("binary")

    if is_mode("debug") then
        add_defines("DEBUG")
        set_symbols("debug")
        set_optimize("none")
        set_warnings("all", "error")
        add_cxflags("-g", "-da", "-Q", {force = true})
    else
        add_defines("NDEBUG")
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end

    add_packages("zlib")
    
    add_includedirs(
        "$(projectdir)/include"
    )

    add_files("source/*.c")