# w60x power plug

1. get `gcc-arm-none-eabi-10-2020-q4-major` [here](https://developer.arm.com/-/media/Files/downloads/gnu-rm/10-2020q4/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2)
2. `python -m venv .env`
3. 进入环境
4. `pip install -r requirements.txt`
5. `pip install -r rt-thread-w60x/requirements.txt`
6. `python control.py rtt update v4.0.3`
7. `python control.py pkgs --upgrade`

### Ver.1
CONFIG_WM_PWM_CH1_PIN=23
CONFIG_WM_PWM_CH2_PIN=22
CONFIG_WM_PWM_CH5_PIN=28

# TODO
LWIP_CHKSUM:

u16_t
lwip_standard_chksum(const void *dataptr, int len)
