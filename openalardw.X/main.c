/* ########################################################################

   Openalardw - Open Firmware para central de alarme Alard Max WiFi
   https://github.com/lcgamboa/openalardw

   ########################################################################

   Copyright (c) : 2020  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#include "config.h"
#include "serial.h"
#include "rf_read.h"
#include "at_serial.h"
#include "itoa.h"
#include "hal.h"
#include "string.h"
#include "eeprom_flash.h"


//prototipos das funcoes 
void ledflash(int vezes, int delay);
int verifica_codigo(unsigned long sensor, unsigned char testa_ctrl);
unsigned char AlarmeOn(void);
unsigned char AlarmeOff(void);
void ApagaLEDs(void);
void LEDOnOff(unsigned char led, unsigned char state);

//variaveis globais
unsigned char AUX_BUZZER;
unsigned char AL_DISP;
unsigned char AL_SETOR;

//variaveis globais locais
static unsigned char AL_ON;
static unsigned char AUX_DISP;
static unsigned char AUX_AD;
static unsigned char gravar;
static unsigned char apagar_conf;
static unsigned char apagar_ctrl;
static unsigned char apagar_sen;
static unsigned char conf_wifi;
static unsigned char wifi_mode;


//programa pricipal

int main() {
    unsigned char AUX_flash = 0;
    unsigned char setor_grava = 0;
    unsigned char AUX_botao = 0;
    char strconv[20];
    rfControl rfControl_RX;
    unsigned long codigo_grava = 0;

    msegundos = 0;
    segundos = 0;
    AL_ON = 0;
    AL_DISP = 0;
    AL_SETOR = 0;
    AUX_BUZZER = 0;
    AUX_DISP = 0;
    AUX_AD = 0;
    gravar = 0;
    apagar_conf = 0;
    apagar_ctrl = 0;
    apagar_sen = 0;
    conf_wifi = 0;
    wifi_mode = 0;

    hal_inicializa_hw();

    //desliga saidas
    EscrevePino(LED1, 0);
    EscrevePino(LED2, 0);
    EscrevePino(LED3, 0);
    EscrevePino(LED4, 0);
    EscrevePino(LED5, 0);
    EscrevePino(LED6, 0);
    EscrevePino(LED7, 0);
    EscrevePino(LED8, 0);
    EscrevePino(LED9, 0);

    EscrevePino(SIRENE, 0);
    EscrevePino(BUZZER, 0);
    EscrevePino(LINE_CON, 0);
    EscrevePino(PGM1, 0);
    EscrevePino(PGM2, 0);
    EscrevePino(PGM3, 0);

    EscrevePino(CTS, 1); //SEND OFF
    EscrevePino(WRESET, 0);
    EscrevePino(WRX, 1);

    //32 segundos
    hal_wdt(1);
    //pisca leds
    ledflash(2, 1000);

    //ler configuracoes flash
    if (eeprom_cfg.SERVER[0] == 0xFF) {
        wifi_mode = NOWIFI;
    } else {
        wifi_mode = WIFIOK;
    }

    //inicializa perifericos
    serial_init();
    hal_InitTimer0();
    hal_InitTimer2();

    //habilita interrupcoes 
    hal_int(1);

    //espera clock estabilizar
    delay_us(100);


    EscrevePino(CTS, 0); //SEND ON

    //reseta modulo wifi
    EscrevePino(WRESET, 1);
    delay_ms(10);
    EscrevePino(WRESET, 0);

    WaitWIND(0, TOUT);

    if (wifi_mode == NOWIFI) {
        SendAT("AT+S.SCFG=sleep_enabled,1", TOUT);
        SendAT("AT+S.SCFG=wifi_powersave,1", TOUT);
        //ret = SendAT("AT+S.SCFG=blink_led,0", TOUT);
        SendAT("AT&W", TOUT);
        SendAT("AT+CFUN=1", TOUT);
        WaitWIND(0, TOUT);
    } else if (wifi_mode == WIFIOK) {
        //desliga mensagens automaticas
        SendAT("AT+S.SCFG=wind_off_low,0xFEFFFFF8", TOUT); //Wind 0:31 mask
        SendAT("AT+S.SCFG=wind_off_medium,0xFFFFFFFF", TOUT); //Wind 32:63 mask
        SendAT("AT+S.SCFG=wind_off_high,0xFFFFFFFF", TOUT); //Wind 64:95 mask
        //configura modulo wifi modo estacao
        SendAT("AT+S.SCFG=sleep_enabled,0", TOUT);
        SendAT("AT+S.SCFG=wifi_powersave,0", TOUT);
        SendAT("AT+S.SCFG=blink_led,1", TOUT);
        /*
        strcpy(atcmd, "AT+S.SSIDTXT=");
        strcat(atcmd, eeprom_cfg.SSID);
        SendAT(atcmd, TOUT);
        strcpy(atcmd, "AT+S.SCFG=wifi_wpa_psk_text,");
        strcat(atcmd, eeprom_cfg.PASSWD);
        SendAT(atcmd, TOUT);
         */
        SendAT("AT+S.SCFG=wifi_opr_rate_mask,0x3FFFCF", TOUT);
        SendAT("AT+S.SCFG=wifi_ht_mode,1", TOUT);
        SendAT("AT+S.SCFG=wifi_priv_mode,2", TOUT);
        SendAT("AT+S.SCFG=wifi_mode,1", TOUT);
        SendAT("AT+S.SCFG=wifi_auth_type,0", TOUT);
        SendAT("AT+S.SCFG=console1_speed,57600", TOUT);
        SendAT("AT+S.SCFG=ip_use_httpd,1", TOUT);
        SendAT("AT+S.SCFG=ip_http_get_recv_timeout,1000", TOUT);
        SendAT("AT&W", TOUT);
        SendAT("AT+CFUN=1", TOUT);

        WaitWIND(0, TOUT);
        //SendAT("AT&V", TOUT);

        WaitWIND(24, TOUT);
        delay_ms(1000); //aguarda wifi estabilizar 
        //verifica status do servidor
        switch (ServerStatus()) {
            case REMOTE_CMD_ON:
                AlarmeOn();
                break;
            case REMOTE_CMD_OFF:
                AlarmeOff();
                break;
            default:
                SendToServer(AL_CONFIG, NULL);
                SendToServer(AL_WIFI_OFF, NULL);
                SendToServer(AL_DISP_OFF, NULL);
                break;
        }

    }


    //pisca leds
    ledflash(10, 100);

    //laco principal
    EscrevePino(CTS, 1); //SEND OFF
    while (1) {
        CLRWDT();
        //verifica se recebeu codigo por RF
        switch (receiveAllCodes(&rfControl_RX)) {
            case RF_RECEIVED: //recebido
                if (gravar) {
                    if (gravar == 1) {
                        gravar++;
                        EscrevePino(LED9, 0);
                        AUX_BUZZER = 1;
                        delay_ms(100);
                        AUX_BUZZER = 0;
                        EscrevePino(LED9, 1);
                        delay_ms(100);
                        AUX_BUZZER = 1;
                        EscrevePino(LED9, 0);
                        delay_ms(100);
                        EscrevePino(LED9, 1);
                        AUX_BUZZER = 0;
                        codigo_grava = rfControl_RX.addr;
                    } else {
                        gravar = 0;

                        if (codigo_grava == rfControl_RX.addr) {

                            //setor_grava -> setores de 1 a 8, 9 ligar e 10 desligar alarme
                            unsigned char sm = 0;
                            //busca ultimo endereco
                            if (setor_grava > 8) {
                                //controles
                                while (((eeprom_ctr[sm] & 0x0FFFFFFFL) != codigo_grava)&&(eeprom_ctr[sm] != 0xFFFFFFFFL)) {
                                    sm++;
                                }

                                if (eeprom_ctr[sm] == 0xFFFFFFFF) //se nao foi encontrado cadastra
                                {
                                    flash_write_dword((unsigned long) &eeprom_ctr[sm], (((unsigned long) setor_grava) << 28) | codigo_grava);
                                }
                            } else {
                                //sensores
                                while (((eeprom_sen[sm] & 0x0FFFFFFFL) != codigo_grava)&&(eeprom_sen[sm] != 0xFFFFFFFFL)) {
                                    sm++;
                                }

                                if (eeprom_sen[sm] == 0xFFFFFFFF) //se nao foi encontrado cadastra
                                {
                                    flash_write_dword((unsigned long) &eeprom_sen[sm], (((unsigned long) setor_grava) << 28) | codigo_grava);
                                }
                            }
                            EscrevePino(LED9, 0);
                            AUX_BUZZER = 1;
                            delay_ms(100);
                            AUX_BUZZER = 0;
                            EscrevePino(LED9, 1);
                            delay_ms(100);
                            AUX_BUZZER = 1;
                            EscrevePino(LED9, 0);
                            delay_ms(100);
                            AUX_BUZZER = 0;
                            if (wifi_mode == WIFIOK) {
                                if (setor_grava > 8) {
                                    ltoha(eeprom_ctr[sm], strconv);
                                } else {
                                    ltoha(eeprom_sen[sm], strconv);
                                }
                                SendToServer(AL_CADASTRO, strconv + 2);
                            }

                        } else {
                            AUX_BUZZER = 1;
                            EscrevePino(LED9, 0);
                            delay_ms(500);
                            AUX_BUZZER = 0;
                        }

                    }
                } else {
                    verifica_codigo(rfControl_RX.addr, 1);
                }
                break;
            case RF_RECEIVING: //recebendo, nao fazer nada ate finalizar
                EscrevePino(CTS, 1); //SEND OFF
                break;
            case RF_IDLE: //esperando
                //habilita recepcao serial, controle de fluxo nao utilizado atualmente
                EscrevePino(CTS, 0); //SEND ON
#ifndef DESLIGAR_SFIO
                if (!gravar) {
                    //verifica entrada de alarme de fio S1
                    if (LePino(S1)) {
                        delay_ms(10); //debounce
                        if (LePino(S1)) {
                            EscrevePino(LED1, 1);
                            AL_DISP = 1;
                            AL_SETOR = 1;
                        }
                    }

                    //verifica entrada de alarme de fio S2
                    if (LePino(S2)) {
                        delay_ms(10); //debounce
                        if (LePino(S2)) {
                            EscrevePino(LED2, 1);
                            AL_DISP = 1;
                            AL_SETOR = 2;
                        }
                    }

                    //verifica entrada de alarme de fio S3
                    if (LePino(S3)) {
                        delay_ms(10); //debounce
                        if (LePino(S3)) {
                            EscrevePino(LED3, 1);
                            AL_DISP = 1;
                            AL_SETOR = 3;
                        }
                    }

                    //verifica entrada de alarme de fio S4
                    if (LePino(S4)) {
                        delay_ms(10); //debounce
                        if (LePino(S4)) {
                            EscrevePino(LED4, 1);
                            AL_DISP = 1;
                            AL_SETOR = 4;
                        }
                    }
                }
#endif
                //verifica entrada de chave A/D
                if (!LePino(AD)) {
                    if (AUX_AD) {
                        delay_ms(10); //debounce
                        if (!LePino(AD)) {
                            AUX_AD = 0;
                            if (AL_ON) {
                                AlarmeOff();
                            } else {
                                AlarmeOn();
                            }
                        }
                    }
                } else {
                    AUX_AD = 1;
                }

                //verifica se o alarme foi disparado e o tempo da sirene
                if (AL_DISP) {
                    if (AL_ON) {
                        AUX_BUZZER = 1;
                        EscrevePino(SIRENE, 1);
                        if (AUX_DISP) {
                            AUX_DISP = 0;
                            inicia_tempo();
                            if (wifi_mode == WIFIOK) {
                                //ltoha(AL_CODE, strconv);
                                utoa2(AL_SETOR, strconv);
                                SendToServer(AL_DISP_ON, strconv);
                            }
                        } else if (segundos >= TSIRENE) {
                            AL_DISP = 0;
                            AUX_DISP = 1;
                            if (wifi_mode == WIFIOK) {
                                SendToServer(AL_DISP_OFF, NULL);
                            }
                        }
                    } else {
                        delay_ms(1000); //TODO melhorar
                        utoa2(AL_SETOR, strconv);
                        SendToServer(AL_SETOR_ON, strconv);
                        AL_DISP = 0;
                        AL_SETOR = 0;
                        ApagaLEDs();
                    }
                } else {
                    AUX_BUZZER = 0;
                    EscrevePino(SIRENE, 0);
                    AUX_DISP = 1;

                    if (AL_ON) {
                        if (!(segundos % 5)) { //envia para o servidor a cada 5 minutos
                            if (AUX_flash) {
                                ledflash(2, 10);
                                AUX_flash = 0;
                            }
                        } else {
                            AUX_flash = 1;
                        }
                    }
                }

                //verifica se o botao foi pressionado
                if (!LePino(BOTAO)) {
                    if (AUX_botao) {
                        delay_ms(10);
                        if (!LePino(BOTAO)) {
                            if (conf_wifi) {
                                RESET();
                            }
                            AUX_botao = 0;
                            if (!gravar) {
                                inicia_tempo();
                                EscrevePino(LED9, 1);
                                AUX_BUZZER = 1;
                                delay_ms(100);
                                AUX_BUZZER = 0;
                                delay_ms(100);
                                AUX_BUZZER = 1;
                                delay_ms(100);
                                AUX_BUZZER = 0;
                                setor_grava = 0;
                                while (!LePino(BOTAO)) //espera enquanto botao estiver pressionado 
                                {
                                    CLRWDT();
                                    if (((segundos > 5)&&(!conf_wifi))) {
                                        conf_wifi = 1;

                                        AUX_BUZZER = 1;
                                        delay_ms(200);
                                        AUX_BUZZER = 0;

                                        ledflash(4, 500);
                                    }

                                    if ((segundos > 10)&&(!apagar_conf)) {
                                        //apaga configuracao
                                        AUX_BUZZER = 1;
                                        delay_ms(200);
                                        AUX_BUZZER = 0;
                                        apagar_conf = 1;
                                        ledflash(4, 500);
                                        flash_erase_block((unsigned long) &eeprom_cfg);
                                    }

                                    if ((segundos > 15)&&(!apagar_ctrl)) {
                                        //apaga controles
                                        AUX_BUZZER = 1;
                                        delay_ms(200);
                                        AUX_BUZZER = 0;
                                        apagar_ctrl = 1;
                                        ledflash(4, 500);
                                        flash_erase_block((unsigned long) &eeprom_ctr);
                                    }

                                    if ((segundos > 20)&&(!apagar_sen)) {
                                        //apaga sensores
                                        AUX_BUZZER = 1;
                                        delay_ms(200);
                                        AUX_BUZZER = 0;
                                        apagar_sen = 1;
                                        ledflash(4, 500);
                                        flash_erase_block((unsigned long) &eeprom_sen);
                                    }

                                }

                                if (segundos < 5) {
                                    gravar = 1;
                                } else if (segundos > 10) {
                                    RESET();
                                } else if (conf_wifi) {

                                    //reseta modulo wifi
                                    EscrevePino(WRESET, 1);
                                    delay_ms(10);
                                    EscrevePino(WRESET, 0);
                                    //configura modulo wifi modo miniAP
                                    WaitWIND(0, TOUT);


                                    SendAT("AT+S.SCFG=sleep_enabled,0", TOUT);
                                    SendAT("AT+S.SCFG=wifi_powersave,0", TOUT);
                                    SendAT("AT+S.SCFG=blink_led,1", TOUT);
                                    SendAT("AT+S.SCFG=wifi_opr_rate_mask,0x3FFFCF", TOUT);
                                    SendAT("AT+S.SCFG=wifi_ht_mode,1", TOUT);
                                    SendAT("AT+S.SCFG=wifi_channelnum,12", TOUT);
                                    SendAT("AT+S.SCFG=console1_speed,57600", TOUT);
                                    SendAT("AT+S.SSIDTXT=ECP_MAX", TOUT);
                                    SendAT("AT+S.SCFG=wifi_wep_keys[0],4543503132", TOUT);
                                    SendAT("AT+S.SCFG=wifi_wep_key_lens,05", TOUT);
                                    SendAT("AT+S.SCFG=wifi_auth_type,1", TOUT);
                                    SendAT("AT+S.SCFG=wifi_priv_mode,1", TOUT);
                                    SendAT("AT+S.SCFG=wifi_mode,3", TOUT);
                                    SendAT("AT+S.SCFG=ip_use_httpd,1", TOUT);

                                    SendAT("AT&W", TOUT);
                                    SendAT("AT+CFUN=1", TOUT);

                                    WaitWIND(0, TOUT);
                                    SendAT("AT&V", TOUT);

                                    //WaitWIND(57, TOUT); //parametros recebidos
                                    //WaitWIND(57, TOUT); //parametros recebidos
                                    //WaitWIND(24, TOUT); //conectado no wifi
                                    //Reset();
                                }
                            }

                            if (gravar) {
                                AUX_BUZZER = 1;
                                delay_ms(100);
                                AUX_BUZZER = 0;


                                setor_grava++;
                                ApagaLEDs();
                                if (setor_grava < 9) {
                                    LEDOnOff(setor_grava, 1);
                                } else if (setor_grava == 9) {
                                    LEDOnOff(1, 1);
                                    LEDOnOff(2, 1);
                                } else if (setor_grava == 10) {
                                    LEDOnOff(1, 1);
                                    LEDOnOff(2, 1);
                                    LEDOnOff(3, 1);
                                }

                                if ((setor_grava > 10) || (gravar > 1)) //finaliza
                                {
                                    gravar = 0;
                                    EscrevePino(LED9, 0);
                                    ApagaLEDs();
                                    AUX_BUZZER = 1;
                                    delay_ms(400);
                                    AUX_BUZZER = 0;
                                }
                            }

                        }
                    }
                } else {
                    AUX_botao = 1;
                }

                if (conf_wifi) {
                    FindConfig(100);
                }

                if (wifi_mode == WIFIOK) {

                    //verifica se recebeu codigo remoto pelo modulo WIFI 
                    switch (FindRemote(10)) {
                        case REMOTE_SENSOR:
                            verifica_codigo(remote, 0);
                            break;
                        case REMOTE_CMD_ON:
                            if (!AL_ON) {
                                AlarmeOn();
                            }
                            break;
                        case REMOTE_CMD_OFF:
                            if (AL_ON) {
                                AlarmeOff();
                            }
                            break;
                        default:
                            break;
                    }

                    if (segundos > TKEEPAL) { //envia para o servidor a cada TKEEPAL segundos
                        SendToServer(AL_KEEP_AL, NULL);
                        inicia_tempo(); //limite segundos em TKEEPAL para evitar estouro
                        //verifica status do servidor
                        switch (ServerStatus()) {
                            case REMOTE_CMD_ON:
                                if (!AL_ON) {
                                    AlarmeOn();
                                }
                                break;
                            case REMOTE_CMD_OFF:
                                if (AL_ON) {
                                    AlarmeOff();
                                }
                                break;
                        }
                    }
                }
                break;
        }
    }

}




//verifica se o codigo recebido e valido 

int verifica_codigo(unsigned long sensor, unsigned char testa_ctrl) {
    unsigned char setor;
    unsigned char sm = 0;

    while (((eeprom_sen[sm] & 0x0FFFFFFFL) != sensor)&&(eeprom_sen[sm] != 0xFFFFFFFFL)) {
        sm++;
    }

    if (eeprom_sen[sm] != 0xFFFFFFFF) {

        setor = (eeprom_sen[sm]&0xF0000000L) >> 28;
    } else {
        if (!testa_ctrl)return 0;
        //busca controle
        sm = 0;
        while (((eeprom_ctr[sm] & 0x0FFFFFFFL) != sensor)&&(eeprom_ctr[sm] != 0xFFFFFFFFL)) {
            sm++;
        }
        if (eeprom_ctr[sm] == 0xFFFFFFFF) return 0;

        setor = (eeprom_ctr[sm]&0xF0000000L) >> 28;
    }

    switch (setor) {
        case 1:
            EscrevePino(LED1, 1);
            break;
        case 2:
            EscrevePino(LED2, 1);
            break;
        case 3:
            EscrevePino(LED3, 1);
            break;
        case 4:
            EscrevePino(LED4, 1);
            break;
        case 5:
            EscrevePino(LED5, 1);
            break;
        case 6:
            EscrevePino(LED6, 1);
            break;
        case 7:
            EscrevePino(LED7, 1);
            break;
        case 8:
            EscrevePino(LED8, 1);
            break;
        case 9:
            if (!AL_ON) {
                AlarmeOn();
            }
            return 0;
            break;
        case 10:
            if (AL_ON) {
                AlarmeOff();
            }
            return 0;
            break;
        default:
            return 0;
            break;
    }
    /*
    if (setor > 8) {
        AL_CODE = eeprom_ctr[sm];
    } else {
        AL_CODE = eeprom_sen[sm];
    }
     */
    AL_SETOR = setor;
    AL_DISP = 1;
    AUX_DISP = 1; //reinicia contagem de tempo
    return 1;
}


//pisca todos os LEDs

void ledflash(int vezes, int delay) {
    int i, j;
    unsigned int LED_status = 0;


    if (LePino(LED1))LED_status |= 0x001;
    if (LePino(LED2))LED_status |= 0x002;
    if (LePino(LED3))LED_status |= 0x004;
    if (LePino(LED4))LED_status |= 0x008;
    if (LePino(LED5))LED_status |= 0x010;
    if (LePino(LED6))LED_status |= 0x020;
    if (LePino(LED7))LED_status |= 0x040;
    if (LePino(LED8))LED_status |= 0x080;
    if (LePino(LED9))LED_status |= 0x100;


    for (i = 0; i < vezes; i++) {
        EscrevePino(LED1, 1);
        EscrevePino(LED2, 1);
        EscrevePino(LED3, 1);
        EscrevePino(LED4, 1);
        EscrevePino(LED5, 1);
        EscrevePino(LED6, 1);
        EscrevePino(LED7, 1);
        EscrevePino(LED8, 1);
        EscrevePino(LED9, 1);
        for (j = 0; j < delay; j++) delay_ms(1);
        EscrevePino(LED1, 0);
        EscrevePino(LED2, 0);
        EscrevePino(LED3, 0);
        EscrevePino(LED4, 0);
        EscrevePino(LED5, 0);
        EscrevePino(LED6, 0);
        EscrevePino(LED7, 0);
        EscrevePino(LED8, 0);
        EscrevePino(LED9, 0);
        for (j = 0; j < delay; j++) delay_ms(1);
    }

    EscrevePino(LED1, (LED_status & 0x001) > 0);
    EscrevePino(LED2, (LED_status & 0x002) > 0);
    EscrevePino(LED3, (LED_status & 0x004) > 0);
    EscrevePino(LED4, (LED_status & 0x008) > 0);
    EscrevePino(LED5, (LED_status & 0x010) > 0);
    EscrevePino(LED6, (LED_status & 0x020) > 0);
    EscrevePino(LED7, (LED_status & 0x040) > 0);
    EscrevePino(LED8, (LED_status & 0x080) > 0);
    EscrevePino(LED9, (LED_status & 0x100) > 0);

}

unsigned char AlarmeOn(void) {

    AL_ON = 1;

    AUX_BUZZER = 1;
    EscrevePino(SIRENE, 1);
    delay_ms(200);
    AUX_BUZZER = 0;
    EscrevePino(SIRENE, 0);
    delay_ms(100);
    AUX_BUZZER = 1;
    EscrevePino(SIRENE, 1);
    delay_ms(200);
    AUX_BUZZER = 0;
    EscrevePino(SIRENE, 0);

    AL_DISP = 0;
    ApagaLEDs();
    gravar = 0;
    AUX_DISP = 1;
    if (wifi_mode == WIFIOK) {
        return SendToServer(AL_WIFI_ON, NULL);
    }
    return 0;
}

unsigned char AlarmeOff(void) {

    AL_ON = 0;

    if (AL_DISP) {
        AL_DISP = 0;
        AUX_BUZZER = 0;
        EscrevePino(SIRENE, 0);
        delay_ms(500);
    }
    AUX_BUZZER = 1;
    EscrevePino(SIRENE, 1);
    delay_ms(200);
    AUX_BUZZER = 0;
    EscrevePino(SIRENE, 0);
    delay_ms(500);
    AUX_BUZZER = 1;
    EscrevePino(SIRENE, 1);
    delay_ms(200);
    AUX_BUZZER = 0;
    EscrevePino(SIRENE, 0);
    delay_ms(500);
    AUX_BUZZER = 1;
    EscrevePino(SIRENE, 1);
    delay_ms(200);
    AUX_BUZZER = 0;
    EscrevePino(SIRENE, 0);
    ApagaLEDs();
    gravar = 0;
    AUX_DISP = 1;
    if (wifi_mode == WIFIOK) {
        /*ret =*/ SendToServer(AL_DISP_OFF, NULL);
        return SendToServer(AL_WIFI_OFF, NULL);
    }
    return 0;
}

void ApagaLEDs(void) {

    EscrevePino(LED1, 0);
    EscrevePino(LED2, 0);
    EscrevePino(LED3, 0);
    EscrevePino(LED4, 0);
    EscrevePino(LED5, 0);
    EscrevePino(LED6, 0);
    EscrevePino(LED7, 0);
    EscrevePino(LED8, 0);

}

void LEDOnOff(unsigned char led, unsigned char state) {
    switch (led) {
        case 1:
            EscrevePino(LED1, state);
            break;
        case 2:
            EscrevePino(LED2, state);
            break;
        case 3:
            EscrevePino(LED3, state);
            break;
        case 4:
            EscrevePino(LED4, state);
            break;
        case 5:
            EscrevePino(LED5, state);
            break;
        case 6:
            EscrevePino(LED6, state);
            break;
        case 7:
            EscrevePino(LED7, state);
            break;
        case 8:
            EscrevePino(LED8, state);
            break;
        case 9:
            EscrevePino(LED9, state);
            break;
        default:
            break;
    }
}
