# Linux kernel driver module and user application

-- (Coded with: C programming language)

this project contains Driver and User interface application:

-- test in Ubuntu OS (Linux)

# Driver
    -- Installation and executing:
        - Step-1: install make
            $ sudo apt-get update -y
            $ sudo apt-get install -y make

        - Step-2: execute make for build driver module
            $ cd Driver
            $ sudo make

        - Step-3: insert driver module
            $ sudo insmod char_dev.ko
            -- check driver log:
                $ sudo dmesg

        - Step-4: create node with major code number that genarate from dmesg log
            $ mknod /dev/char_dev c <major number> 0

# user app
    note: check your ubuntu has gcc > 7
    -- Installation and executing:
        - Step-1: build user app
            $ sudo gcc userapp.c -o userapp
            $ chmod 777 userapp

        - Step-2: run app
            $ sudo ./userapp
