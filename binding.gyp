{
    "targets": [
        {
            "target_name": "awesome-lib",
            'defines': ['V8_DEPRECATION_WARNINGS=1'],
            "sources": [
                "awesome-lib.h",
                "addon.cc"
            ],
            "libraries": ["../awesome-lib.so"],
            'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
            'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
            'cflags!': ['-fno-exceptions'],
            'cflags_cc!': ['-fno-exceptions'],
            'xcode_settings': {
                'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                'CLANG_CXX_LIBRARY': 'libc++',
                'MACOSX_DEPLOYMENT_TARGET': '10.7',
            },
            'msvs_settings': {
                'VCCLCompilerTool': {
                    'ExceptionHandling': 1
                },
            },
        }
    ]
}