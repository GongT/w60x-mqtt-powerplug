# w60x power plug

1. get `gcc-arm-none-eabi-10-2020-q4-major`
2. `python -m venv .env`
3. `./.env/bin/pip install -r requirements.txt`
3. `./.env/bin/pip install -r rt-thread-w60x/requirements.txt`

### Ver.1
CONFIG_WM_PWM_CH1_PIN=23
CONFIG_WM_PWM_CH2_PIN=22
CONFIG_WM_PWM_CH5_PIN=28

# TODO
LWIP_CHKSUM:

u16_t
lwip_standard_chksum(const void *dataptr, int len)
