import os

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))

include_dirs = [
    '.',
]


flags = [
    '-DEBUG',
    '-std=c++11',
    '-pthread',
]

def FlagsForFile( filename, **kwargs ):
    """
    Given a source file, retrieves the flags necessary for compiling it.
    """
    for dir in include_dirs:
        flags.append('-I' + os.path.join(CURRENT_DIR, dir))

    return { 'flags': flags }
