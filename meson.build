project(
    'c-bar',
    'c',
    default_options: [
        'c_std=c23',
        'optimization=0',
        'warning_level=3'
    ]
)

add_project_dependencies([
    dependency('gtk+-3.0'),
    dependency('gtk-layer-shell-0'),
    dependency('libcjson'),
], language: 'c')

add_project_arguments([
    '-DGDK_DISABLE_DEPRECATED',
    '-DGTK_DISABLE_DEPRECATED',
], language: 'c')

executable(
    'c-bar',
    files(
        'src/bar.c',
        'src/main.c',
        'src/modules.c',
        'src/utils.c',
    ),
    include_directories: include_directories('include'),
)