# Meson build file for Game of Life Plymouth plugin
# Place this in plymouth/src/plugins/splash/gameoflive/ if building with Plymouth source

gameoflive_splash = shared_library(
    'gameoflive',
    sources: [
        'plugin.c',
        '../../../bitmap.c',
        '../../../data.c',
        '../../../draw.c',
    ],
    include_directories: [
        inc,
        include_directories('../../../'),
    ],
    dependencies: [
        plymouth_deps,
    ],
    install: true,
    install_dir: plymouth_plugin_path,
)
