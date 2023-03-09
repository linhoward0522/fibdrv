CONFIG_MODULE_SIG = n
TARGET_MODULE := fibdrv

obj-m := $(TARGET_MODULE).o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

GIT_HOOKS := .git/hooks/applied

all: $(GIT_HOOKS) client client_test
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	$(RM) client out client_test test

load:
	sudo insmod $(TARGET_MODULE).ko
unload:
	sudo rmmod $(TARGET_MODULE) || true >/dev/null

client: client.c
	$(CC) -o $@ $^

client_test: client_test.c
	$(CC) -o $@ $^
 
PRINTF = env printf
PASS_COLOR = \e[32;01m
NO_COLOR = \e[0m
pass = $(PRINTF) "$(PASS_COLOR)$1 Passed [-]$(NO_COLOR)\n"

check: all
	$(MAKE) unload
	$(MAKE) load
	sudo ./client > out
	$(MAKE) unload
	@diff -u out scripts/expected.txt && $(call pass)
	@scripts/verify.py

test: all
	$(MAKE) unload
	$(MAKE) load
	sudo taskset -c 7 ./client_test 4 > test
	gnuplot scripts/test.gp
	sudo taskset -c 7 ./client_test 1 > test_time
	gnuplot scripts/test_time.gp
	$(MAKE) unload
	