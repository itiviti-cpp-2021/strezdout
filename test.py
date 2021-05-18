#!/usr/bin/env python3
import sys
import os
import argparse
import subprocess

def create_argparser():
    parser = argparse.ArgumentParser(
        description="Stress tests",
        epilog="By @renbou with <3",
    )
    parser.add_argument("task", help="Which task to test",
                                choices=["wordnet", "2d-tree", "8-puzzle", "genome-assembly"])
    parser.add_argument("sources", help="Path to source directory")
    parser.add_argument("includes", help="Path to include directory")
    parser.add_argument("-r", "--rebuild", dest="rebuild", default=False, action='store_true',
                        help="Rebuild the image if an update is needed")
    return parser

def run_with_stdout(cmd):
  process = subprocess.run(
    cmd,
    stdout=sys.stdout,
    stderr=subprocess.STDOUT,
    shell=True
  )
  return process.returncode

def main(args):
    for name in ["sources", "includes"]:
        setattr(args, name, os.path.realpath(getattr(args, name)))
        if not os.path.exists(getattr(args, name)):
            print("Path to {} ({}) does not exist".format(name, getattr(args, name)))
            sys.exit(1)
    if args.rebuild:
        run_with_stdout("docker image rm strezdout:1.0")
    if args.rebuild or b'strezdout' not in \
            subprocess.check_output("docker images --filter reference='strezdout:1.0'", shell=True):
        run_with_stdout("docker build . --tag strezdout:1.0")
    run_with_stdout("docker run -ti --volume {}:/src --volume {}:/include --rm strezdout:1.0 {}"
                        .format(args.sources, args.includes, args.task))


if __name__ == '__main__':
    argparser = create_argparser()
    main(argparser.parse_args())
