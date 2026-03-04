
import sys, time
import util
import pytest


def crash_log() -> None:
    q = util.QEMU(True)
    q.cmd("logstress f0 f1 f2 f3 f4 f5\n")
    time.sleep(2)
    q.crash()
    q.stop()


def recover_log() -> None:
    q = util.QEMU()
    time.sleep(2)
    q.read()
    ok, _ = q.match('^recovering', exit=False)
    if ok:
        q.cmd("ls\n")
        time.sleep(2)
        q.read()
        q.match('f5')
    q.stop()
    return ok


def forphan() -> None:
    q = util.QEMU(True)
    q.cmd("forphan\n")
    time.sleep(5)
    q.read()
    q.match('.*wait.*')
    q.crash()
    q.stop()


def dorphan() -> None:
    q = util.QEMU(True)
    q.cmd("dorphan\n")
    time.sleep(5)
    q.read()
    q.match('.*wait.*')
    q.crash()
    q.stop()


def recover_orphan() -> None:
    q = util.QEMU()
    time.sleep(2)
    q.read()
    q.match('^ireclaim')
    q.stop()


def test_log() -> None:
    print("Test recovery of log")
    for i in range(5):
        crash_log()
        ok = recover_log()
        if ok:
            print("OK")
            return
        print("log attempt ", i+1)
    print("FAIL")
    sys.exit(1)
    

def test_forphan() -> None:
    forphan()
    recover_orphan()


def test_dorphan() -> None:
    dorphan()
    recover_orphan()


def test_crash() -> None:
    test_log()
    test_forphan()
    test_dorphan()


@pytest.mark.parametrize(
  "test_name", 
  [
    "test_lseek_set",
    "test_lseek_cur",
    "test_lseek_end",
    "sprintf_string",
    "sprintf_integer",
    "sprintf_integer_newline",
    "copyin",
    "copyout",
    "copyinstr1",
    "copyinstr2",
    "copyinstr3",
    "rwsbrk" ,
    "truncate1",
    "truncate2",
    "truncate3",
    "openiput",
    "exitiput",
    "iput",
    "opentest",
    "writetest",
    "writebig",
    "createtest",
    "dirtest",
    "exectest",
    "pipe1",
    "killstatus",
    "preempt",
    "exitwait",
    "reparent" ,
  ],
)
def test_usertests(test_name: str) -> None:
    timeout = 600
    q = util.QEMU(True)
    q.cmd(f"usertests {test_name}\n")
    q.monitor('^ALL TESTS PASSED', progress='test', timeout=timeout)
    q.stop()


def test_hello() -> None:
    q = util.QEMU(True)
    q.cmd("hello\n")
    time.sleep(1)
    q.read()
    lines = q.lines()
    assert 'Hello, World!' in lines
    q.stop()


def test_find() -> None:
    q = util.QEMU(True)
    q.cmd("find /bin/\n")
    time.sleep(1)
    q.read()
    lines = q.lines()
    assert '/bin/hello' in lines
    assert '/bin/wc' in lines
    q.stop()


def test_time() -> None:
    q = util.QEMU(True)
    q.cmd("time sleep 1\n")
    time.sleep(2)
    q.read()
    lines = q.lines()
    assert " total" in lines[-2]
    q.stop()


def test_pingpong() -> None:
    q = util.QEMU(True)
    q.cmd("pingpong\n")
    time.sleep(2)
    q.read()
    lines = q.lines()

    # not testing latest line because it is likely to
    # get cut off before completion     
    assert "pingpong" in lines[-2]
    assert "pingpong" in lines[-3]
    q.stop()


def test_pingpong_max_iter() -> None:
    q = util.QEMU(True)
    q.cmd("pingpong 100\n")
    time.sleep(2)
    q.read()
    lines = q.lines()
    assert len([line for line in lines if line == 'pingpong']) == 100
    q.stop()


def test_meminfo() -> None:
    q = util.QEMU(True)
    q.cmd("cat /dev/meminfo\n")
    time.sleep(2)
    q.read()
    lines = q.lines()
    assert "FreePages: " in lines[-3]
    assert "MemFree: " in lines[-2]
    q.stop()


def test_append() -> None:
    q = util.QEMU(True)
    q.cmd("echo hello > testfile\n")
    q.cmd("echo world >> testfile\n")
    q.cmd("cat testfile\n")
    time.sleep(2)
    q.read()
    lines = q.lines()
    assert "hello" in lines[-3]
    assert "world" in lines[-2]
    q.stop()
