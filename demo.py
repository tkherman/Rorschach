#!/usr/bin/env python2.7

# Usage: ./demo.py [demo...]
#
# Where demo can be any of the following:
#
#   - make          Demo building and cleaning
#   - args          Demo parsing arguments
#   - logs          Demo logging
#   - rule          Demo rule parsing
#   - patt          Demo pattern matching
#   - exec          Demo executing actions
#
# If no arguments are passed, then all the demos are ran.

import atexit
import contextlib
import glob
import os
import random
import shlex
import shutil
import signal
import subprocess
import sys
import time

# Constants --------------------------------------------------------------------

AFS_PYTHON_PATH = '/afs/nd.edu/user15/pbui/pub/anaconda2-4.1.1/bin'
TIMESTAMP       = int(time.time())
DEMO_LOG        = '{}.demo.log'.format(TIMESTAMP)
DEMO_ROOT       = '{}.demo.root'.format(TIMESTAMP)
DEMO_RULES      = '{}.demo.rules'.format(TIMESTAMP)
DEMO_TIMEOUT    = 1
DEV_NULL        = open(os.devnull)

# DemoRoot ---------------------------------------------------------------------

@contextlib.contextmanager
def DemoRoot(path=DEMO_ROOT):
    if os.path.exists(path):
        shutil.rmtree(path)

    os.makedirs(path)

    yield path

    shutil.rmtree(path)

# Cleanup ----------------------------------------------------------------------

def cleanup():
    for path in (DEMO_LOG, DEMO_RULES, DEMO_ROOT):
        if os.path.exists(path):
            os.unlink(path)

# Process Utility --------------------------------------------------------------

def create_process(command, stdout=DEV_NULL, stderr=DEV_NULL, timeout=DEMO_TIMEOUT):
    try:
        p = subprocess.Popen(command, shell=True, stdout=stdout, stderr=stderr, preexec_fn=os.setsid)
        time.sleep(timeout)
        p.poll()
    except OSError as e:
        print e

    return p

# Cowsay -----------------------------------------------------------------------

def cowsay(msg, cowfile='default', timeout=1,before=None,after=''):
    if before is not None:
        print before
    process = subprocess.Popen(['cowsay', '-n', '-f', cowfile], stdin=subprocess.PIPE)
    process.communicate(msg)
    process.wait()
    time.sleep(timeout)
    print after

POSITIVE = (
    'Great success!',
    'Hooray!',
    'Nice!',
    'Wahoo!',
    'Cool!',
)

def positive_cowsay(msg, *args):
    cowsay(random.choice(POSITIVE) + ' ' + msg, before='\033[92m', after='\033[0m', *args)

NEGATIVE = (
    'Oh snap ...',
    'Uh oh ...',
    'Sorry ...',
    'Sigh ...',
    'Welp ...',
)

def negative_cowsay(msg, *args):
    cowsay(random.choice(NEGATIVE) + ' ' + msg, 'cower', before='\033[91m', after='\033[0m', *args)

def warning_cowsay(msg, *args, **kwargs):
    cowsay(msg, before='\033[93m', after='\033[0m', *args, **kwargs)

# Make demo --------------------------------------------------------------------

def do_make_demo():
    warning_cowsay('''
This is the make demo.  It will attempt to clean and build your project using
`make`.  Here we go ...
''', timeout=5)

    cowsay('Executing `make clean` ...')
    if os.system('make clean 2>&1 | tee {}'.format(DEMO_LOG)) != 0:
        negative_cowsay("Looks like `make clean` failed", 'cower')
    elif len(glob.glob('*.o')) or os.path.exists('rorschach'):
        negative_cowsay("Looks like `make clean` missed some files", 'cower')
    else:
        positive_cowsay('Looks like `make clean` worked.')

    cowsay('Executing `make` ...')
    if os.system('make 2>&1 | tee {}'.format(DEMO_LOG)) != 0:
        negative_cowsay("Looks like we have some compilation errors", 'cower')
    elif 'warning' in open(DEMO_LOG).read().lower():
        negative_cowsay("Looks like we have some compilation warnings", 'cower')
    elif not os.path.exists('rorschach'):
        negative_cowsay("Looks like we didn't build `rorschach`", 'cower')
    else:
        positive_cowsay('Looks like `make` worked.')

# Arguments demo ---------------------------------------------------------------

def do_args_demo():
    warning_cowsay('''
This is the arguments demo.  It will attempt to see if `rorschach` can at least
parse or understand the different command line arguments.

Note: it does not check if the arguments are handled properly.  It just tests
if you can pass them without errors.

Here we go ...
''', timeout=10)

    command = './rorschach -h'
    cowsay('Executing `{}` ...'.format(command))
    if os.system('./rorschach -h 2>&1 | grep -q -i usage') != 0:
        negative_cowsay('Looks like you didn\'t display a usage message')
    else:
        positive_cowsay('Looks like the help flag was handled successfully.')

    command = './rorschach -t {}'.format(DEMO_TIMEOUT)
    cowsay('Executing `{}` ...'.format(command))
    process = create_process(command)
    if process.returncode is None:
        os.killpg(os.getpgid(process.pid), signal.SIGTERM)
        positive_cowsay('Looks like the timeout flag was handled successfully.')
    else:
        negative_cowsay('Looks like the timeout flag was not handled properly.')

    open(DEMO_RULES, 'w')
    command = './rorschach -f {}'.format(DEMO_RULES)
    cowsay('Executing `{}` ...'.format(command))
    process = create_process(command)
    if process.returncode is None:
        os.killpg(os.getpgid(process.pid), signal.SIGTERM)
        positive_cowsay('Looks like the rules flag was handled successfully.')
    else:
        negative_cowsay('Looks like the rules flag was not handled properly.')

# Logging demo ----------------------------------------------------------------

def do_logs_demo():
    warning_cowsay('''
This is the logging demo.  It will attempt to see if `rorschach` logs events
and operations properly.

Note: it does not check if events are handled properly, just if they are
logged.

Here we go ...
''', timeout=10)

    with open(DEMO_RULES, 'w') as fs:
        fs.write('''
CREATE  *   true
MODIFY  *   true
DELETE  *   true
''')

    cowsay('''
We will be starting `rorschach` in the background and run it with the following
set of custom rules:

{}
'''.format(open(DEMO_RULES).read()))

    with DemoRoot():
        command = 'stdbuf -o0 ./rorschach -t {} -f {} {} | tee {}'.format(
            DEMO_TIMEOUT, DEMO_RULES, DEMO_ROOT, DEMO_LOG)

        cowsay('Executing `{}` ...'.format(command))
        process = create_process(command, stdout=None, stderr=None, timeout=0)
        time.sleep(2)

        test_file = os.path.join(DEMO_ROOT, '_POLLEN_AND_SALT_')
        cowsay('Creating `{}` ...'.format(test_file))
        open(test_file, 'w').close()
        time.sleep(2)

        cowsay('Modifying `{}` ...'.format(test_file))
        open(test_file, 'w').close()
        time.sleep(2)

        cowsay('Deleting `{}` ...'.format(test_file))
        os.unlink(test_file)
        time.sleep(2)

        os.killpg(os.getpgid(process.pid), signal.SIGTERM)

    has_monitoring = False
    has_detection  = False
    has_matching   = False
    has_executing  = False
    has_create     = 0
    has_modify     = 0
    has_delete     = 0
    for line in open(DEMO_LOG):
        line = line.lower().strip()
        has_monitoring = has_monitoring or line.startswith('monitoring')
        has_detection  = has_detection  or line.startswith('detected')
        has_matching   = has_matching   or line.startswith('matched')
        has_executing  = has_executing  or line.startswith('executing')
        has_create    += 1 if '"create" event' in line and test_file.lower() in line else 0
        has_modify    += 1 if '"modify" event' in line and test_file.lower() in line else 0
        has_delete    += 1 if '"delete" event' in line and test_file.lower() in line else 0

    messages = ['Here are the results of the logging demo:', '']

    message = '- {:25} ... '.format('Periodic scan')
    if all([has_monitoring, has_detection, has_matching, has_executing]):
        message += 'Success'
    else:
        message += 'Missing logging messages'
    messages.append(message)

    message = '- {:25} ... '.format('Detects CREATE events')
    if has_create == 1:
        message += 'Success'
    else:
        message += 'Wrong amount of CREATE events'
    messages.append(message)

    message = '- {:25} ... '.format('Detects MODIFY events')
    if has_modify == 1:
        message += 'Success'
    else:
        message += 'Wrong amount of MODIFY events'
    messages.append(message)

    message = '- {:25} ... '.format('Detects DELETE events')
    if has_delete == 1:
        message += 'Success'
    else:
        message += 'Wrong amount of DELETE events'
    messages.append(message)

    warning_cowsay('\n'.join(messages))

# Rules demo ------------------------------------------------------------------

def do_rule_demo():
    warning_cowsay('''
This is the rules demo.  It will attempt to see how `rorschach` handles parsing
different rules.

Note: it does not check if events are handled properly, just if the parsing
behaves properly.

Here we go ...
''', timeout=10)

    # Valid rules
    handles_valid_rules = True
    with open(DEMO_RULES, 'w') as fs:
        fs.write('''
CREATE  *.txt   echo _SO_RUDE_
MODIFY  *.txt   echo _WELCOME_HOME_
DELETE  *.txt   echo _HARD_TO_EXPLAIN_
''')
    cowsay('''
We will be starting `rorschach` in the background and run it with the following
set of custom rules:

{}
'''.format(open(DEMO_RULES).read()), timeout=3)

    with DemoRoot():
        command = './rorschach -t {} -f {} {}'.format(DEMO_TIMEOUT, DEMO_RULES, DEMO_ROOT)
        process = create_process(command, timeout=1)
        if process.returncode is None:
            os.killpg(os.getpgid(process.pid), signal.SIGTERM)
            positive_cowsay('Looks like you can handle valid rules')
        else:
            handles_valid_rules = False
            negative_cowsay('Looks like you cannot handle valid rules')

    # Invalid rules
    handles_invalid_rules = False
    with open(DEMO_RULES, 'w') as fs:
        fs.write('''
CREATE
MODIFY  *.txt
MOOGLE  *       true
''')
    cowsay('''
We will be starting `rorschach` in the background and run it with the following
set of custom rules:

{}
'''.format(open(DEMO_RULES).read()), timeout=3)
    with DemoRoot():
        command = './rorschach -t {} -f {} {}'.format(DEMO_TIMEOUT, DEMO_RULES, DEMO_ROOT)
        process = create_process(command, timeout=1)
        if process.returncode is None:
            os.killpg(os.getpgid(process.pid), signal.SIGTERM)
            negative('Looks like you cannot handle invalid rules')
        else:
            handles_invalid_rules = True
            positive_cowsay('Looks like you can handle invalid rules')

    # Empty lines
    handles_empty_lines = True
    with open(DEMO_RULES, 'w') as fs:
        fs.write('''
CREATE  *       echo _SO_RUDE_

MODIFY  *       echo _WELCOME_HOME_

DELETE  *       echo _HARD_TO_EXPLAIN_
''')
    cowsay('''
We will be starting `rorschach` in the background and run it with the following
set of custom rules:

{}
'''.format(open(DEMO_RULES).read()), timeout=3)
    with DemoRoot():
        command = './rorschach -t {} -f {} {}'.format(DEMO_TIMEOUT, DEMO_RULES, DEMO_ROOT)
        process = create_process(command, timeout=1)
        if process.returncode is None:
            os.killpg(os.getpgid(process.pid), signal.SIGTERM)
            positive_cowsay('Looks like you can handle empty lines')
        else:
            handles_empty_lines = False
            negative_cowsay('Looks like you cannot handle empty lines')

    # Comments
    handles_comments = True
    with open(DEMO_RULES, 'w') as fs:
        fs.write('''
# Comment
CREATE  *       echo _SO_RUDE_
# Comment
MODIFY  *       echo _WELCOME_HOME_
# Comment
DELETE  *       echo _HARD_TO_EXPLAIN_
# Comment
''')
    cowsay('''
We will be starting `rorschach` in the background and run it with the following
set of custom rules:

{}
'''.format(open(DEMO_RULES).read()), timeout=3)
    with DemoRoot():
        command = './rorschach -t {} -f {} {}'.format(DEMO_TIMEOUT, DEMO_RULES, DEMO_ROOT)
        process = create_process(command, timeout=1)
        if process.returncode is None:
            os.killpg(os.getpgid(process.pid), signal.SIGTERM)
            positive_cowsay('Looks like you can handle comments')
        else:
            handles_comments = False
            negative_cowsay('Looks like you cannot handle comments')

    # Variables
    handles_variables = True
    with open(DEMO_RULES, 'w') as fs:
        fs.write('''
CREATE  *       echo ${FULLPATH}
MODIFY  *       echo ${BASEPATH}
DELETE  *       echo ${TIMESTAMP} ${EVENT}
''')
    cowsay('''
We will be starting `rorschach` in the background and run it with the following
set of custom rules:

{}
'''.format(open(DEMO_RULES).read()), timeout=3)
    with DemoRoot():
        command = './rorschach -t {} -f {} {}'.format(DEMO_TIMEOUT, DEMO_RULES, DEMO_ROOT)
        process = create_process(command, timeout=1)
        if process.returncode is None:
            os.killpg(os.getpgid(process.pid), signal.SIGTERM)
            positive_cowsay('Looks like you can handle variables')
        else:
            handles_variables = False
            negative_cowsay('Looks like you cannot handle variables')

    messages = ['Here are the results of the rules demo:', '']

    message = '- {:25} ... '.format('Handles Valid rules')
    if handles_valid_rules:
        message += 'Success'
    else:
        message += 'Failure'
    messages.append(message)

    message = '- {:25} ... '.format('Handles Invalid rules')
    if handles_invalid_rules:
        message += 'Success'
    else:
        message += 'Failure'
    messages.append(message)

    message = '- {:25} ... '.format('Handles Empty Lines')
    if handles_empty_lines:
        message += 'Success'
    else:
        message += 'Failure'
    messages.append(message)

    message = '- {:25} ... '.format('Handles Comments')
    if handles_comments:
        message += 'Success'
    else:
        message += 'Failure'
    messages.append(message)

    message = '- {:25} ... '.format('Handles Variables')
    if handles_variables:
        message += 'Success'
    else:
        message += 'Failure'
    messages.append(message)

    warning_cowsay('\n'.join(messages))

# Pattern demo ----------------------------------------------------------------

def do_patt_demo():
    warning_cowsay('''
This is the pattern demo.  It will attempt to see how `rorschach` handles
matching with different patterns.

Note: it does not check if events are handled properly, just if the patterns
are matched.

Here we go ...
''', timeout=10)

    with open(DEMO_RULES, 'w') as fs:
        fs.write('''
CREATE  *BONES* echo bones
CREATE  *.jpg   echo jpeg
CREATE  *.txt   echo text
CREATE  hello   echo hi
''')
    with DemoRoot():
        command = 'stdbuf -o0 ./rorschach -t {} -f {} {} | tee {}'.format(
            DEMO_TIMEOUT, DEMO_RULES, DEMO_ROOT, DEMO_LOG)

        cowsay('Executing `{}` ...'.format(command))
        process = create_process(command, stdout=None, stderr=None, timeout=2)

        test_file = os.path.join(DEMO_ROOT, '_SEE_THESE_BONES_')
        cowsay('Creating `{}` ...'.format(test_file))
        open(test_file, 'w').close()
        time.sleep(2)

        test_file = os.path.join(DEMO_ROOT, 'good_riddance.txt')
        cowsay('Creating `{}` ...'.format(test_file))
        open(test_file, 'w').close()
        time.sleep(2)

        test_file = os.path.join(DEMO_ROOT, 'lonesome_dreams.jpg')
        cowsay('Creating `{}` ...'.format(test_file))
        open(test_file, 'w').close()
        time.sleep(2)

        test_file = os.path.join(DEMO_ROOT, 'hello')
        cowsay('Creating `{}` ...'.format(test_file))
        open(test_file, 'w').close()
        time.sleep(2)

        os.killpg(os.getpgid(process.pid), signal.SIGTERM)

    has_bones    = False
    has_riddance = False
    has_dreams   = False
    has_hello    = False
    for line in open(DEMO_LOG):
        line = line.lower().strip()
        if not line.startswith('matched'):
            continue
        has_bones    = has_bones    or '_see_these_bones_'   in line
        has_riddance = has_riddance or 'good_riddance.txt'   in line
        has_dreams   = has_dreams   or 'lonesome_dreams.jpg' in line
        has_hello    = has_hello    or 'hello'               in line

    if all([has_bones, has_riddance, has_dreams, has_hello]):
        positive_cowsay('Looks like you matched all the patterns.')
    else:
        negative_cowsay('Looks like you missed some patterns.')

# Execution demo --------------------------------------------------------------

def do_exec_demo():
    warning_cowsay('''
This is the execution demo.  It will attempt to see how `rorschach` executes
actions with environmental variables.

Here we go ...
''', timeout=10)

    with open(DEMO_RULES, 'w') as fs:
        fs.write('''
CREATE  DYING_ISNT_EASY         touch {0}/YOU_LEFT_ME_JUST_WHEN_I_NEEDED_YOU
MODIFY  DYING_ISNT_EASY         touch {0}/YOU_SET_ME_BACK_WHEN_I_THOUGHT_I_GOT_BACK
DELETE  DYING_ISNT_EASY         touch {0}/JUST_WHEN_I_THOUGHT_I_GOT_BACK
CREATE  number.5                echo ${{FULLPATH}} >  {0}/with_a_bullet
MODIFY  number.5                echo ${{BASEPATH}} >> {0}/with_a_bullet
DELETE  number.5                echo ${{EVENT}} ${{TIMESTAMP}} >> {0}/with_a_bullet
'''.format(DEMO_ROOT))
    with DemoRoot():
        command = 'stdbuf -o0 ./rorschach -t {} -f {} {} | tee {}'.format(
            DEMO_TIMEOUT, DEMO_RULES, DEMO_ROOT, DEMO_LOG)

        cowsay('Executing `{}` ...'.format(command))
        process = create_process(command, stdout=None, stderr=None, timeout=2)

        test_file = os.path.join(DEMO_ROOT, 'DYING_ISNT_EASY')
        cowsay('Creating `{}` ...'.format(test_file))
        open(test_file, 'w').close()
        time.sleep(2)

        cowsay('Modifying `{}` ...'.format(test_file))
        open(test_file, 'w').close()
        time.sleep(2)

        cowsay('Deleting `{}` ...'.format(test_file))
        os.unlink(test_file)
        time.sleep(2)

        test_file = os.path.join(DEMO_ROOT, 'number.5')
        cowsay('Creating `{}` ...'.format(test_file))
        open(test_file, 'w').close()
        time.sleep(2)

        cowsay('Modifying `{}` ...'.format(test_file))
        open(test_file, 'w').close()
        time.sleep(2)

        cowsay('Deleting `{}` ...'.format(test_file))
        os.unlink(test_file)
        time.sleep(2)

        executes_actions = all([
            os.path.exists(os.path.join(DEMO_ROOT, 'YOU_LEFT_ME_JUST_WHEN_I_NEEDED_YOU')),
            os.path.exists(os.path.join(DEMO_ROOT, 'YOU_SET_ME_BACK_WHEN_I_THOUGHT_I_GOT_BACK')),
            os.path.exists(os.path.join(DEMO_ROOT, 'JUST_WHEN_I_THOUGHT_I_GOT_BACK')),
        ])

        with open(os.path.join(DEMO_ROOT, 'with_a_bullet')) as fs:
            handles_env_vars = all([
                fs.readline().strip() == os.path.join(DEMO_ROOT, 'number.5'),
                fs.readline().strip() == 'number.5',
                fs.readline().strip().startswith('DELETE'),
            ])

        os.killpg(os.getpgid(process.pid), signal.SIGTERM)

    if executes_actions and handles_env_vars:
        positive_cowsay('Looks like you executed all the actions successfully.')
    else:
        negative_cowsay('Looks like you did not execute all the actions successfully.')

# Signal demo -----------------------------------------------------------------

def do_sigl_demo():
    warning_cowsay('''
This is the signal demo.  It will attempt to see how `rorschach` handles the
`SIGINT` signal.

Here we go ...
''', timeout=10)
    open(DEMO_RULES, 'w').close()

    with DemoRoot():
        with open(DEMO_LOG, 'w') as fs:
            command = 'stdbuf -o0 ./rorschach -t {} -f {} {}'.format(
                DEMO_TIMEOUT, DEMO_RULES, DEMO_ROOT)

            cowsay('Executing `{}` ...'.format(command))
            process = subprocess.Popen(command.split(), stdout=fs, stderr=fs)
            time.sleep(1)

            cowsay('Sending `SIGINT`')
            os.kill(process.pid, signal.SIGINT)
            time.sleep(1)

    process.poll()

    if process.returncode == 0 and 'bye' in open(DEMO_LOG).readlines()[-1].lower():
        positive_cowsay('Looks like you handled SIGINT successfully.')
    else:
        process.kill()
        negative_cowsay('Looks like you did not handle SIGINT successfully.')

# Valgrind demo ---------------------------------------------------------------

def do_valg_demo():
    warning_cowsay('''
This is the valgrind demo.  It will attempt to see how `rorschach` handles
memory.

Here we go ...
''', timeout=10)

    with open(DEMO_RULES, 'w') as fs:
        fs.write('''
CREATE  SUN_HANDS true
MODIFY  SUN_HANDS echo ${EVENT} ${BASEPATH}
DELETE  SUN_HANDS echo ${TIMESTAMP} ${FULLPATH}
''')
    with DemoRoot():
        with open(DEMO_LOG, 'w') as fs:
            command = 'stdbuf -o0 valgrind --leak-check=full ./rorschach -t {} -f {} {}'.format(
                DEMO_TIMEOUT, DEMO_RULES, DEMO_ROOT)

            cowsay('Executing `{}` ...'.format(command))
            process = subprocess.Popen(command.split(), stdout=fs, stderr=fs)
            time.sleep(1)

            test_file = os.path.join(DEMO_ROOT, 'SUN_HANDS')
            cowsay('Creating `{}` ...'.format(test_file))
            open(test_file, 'w').close()
            time.sleep(2)

            cowsay('Modifying `{}` ...'.format(test_file))
            open(test_file, 'w').close()
            time.sleep(2)

            cowsay('Deleting `{}` ...'.format(test_file))
            os.unlink(test_file)
            time.sleep(2)

            os.kill(process.pid, signal.SIGINT)
            time.sleep(2)
            process.poll()

    if process.returncode is None:
        os.kill(process.pid, signal.SIGTERM)

    if 'ERROR SUMMARY: 0 errors from 0 contexts' in open(DEMO_LOG).read():
        positive_cowsay('Looks like you handle memory properly.')
    else:
        negative_cowsay('Looks like you did not handle memory properly.')

# Main execution ---------------------------------------------------------------

if __name__ == '__main__':
    os.environ['PATH'] = '{}:{}'.format(AFS_PYTHON_PATH, os.environ['PATH'])

    DEMOS = {
        'make'  : (0, do_make_demo),
        'args'  : (1, do_args_demo),
        'logs'  : (2, do_logs_demo),
        'rule'  : (3, do_rule_demo),
        'patt'  : (4, do_patt_demo),
        'exec'  : (5, do_exec_demo),
        'sigl'  : (6, do_sigl_demo),
        'valg'  : (7, do_valg_demo),
    }

    atexit.register(cleanup)

    if len(sys.argv) == 1:
        demos = sorted(DEMOS.keys(), key=lambda k: DEMOS[k][0])
    else:
        demos = sys.argv[1:]

    for demo in demos:
        try:
            DEMOS[demo][1]()
        except KeyError:
            print 'demo needs to be one of: {}'.format(', '.join(sorted(DEMOS.keys())))
