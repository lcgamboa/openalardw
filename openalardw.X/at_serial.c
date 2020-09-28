

#include "at_serial.h"
#include "config.h"
#include "serial.h"
#include "string.h"
#include "itoa.h"
#include "eeprom_flash.h"
#include "hal.h"

extern unsigned char AUX_BUZZER;
extern unsigned char AL_DISP;
extern unsigned char AL_SETOR;

static char buff[1024];
static char strcmd[1024];
static char itoabuff[20];

unsigned char SendAT(const char * cmd, int timeout) {
    serial_tx('A');
    serial_clear_buffer();
    serial_tx_str(cmd + 1);
    serial_tx('\r');

    serial_rx_str_until(buff, 1024, '\n', timeout);

    serial_rx_str_until(buff, 1024, '\n', timeout);

    return strcmp(buff, "OK\r\n");
}

unsigned char WaitWIND(unsigned char num, int timeout) {
    unsigned char wind = 0xFF;
    unsigned char count = 0;
    do {
        buff[0] = 0;
        serial_rx_str_until(buff, 1024, '\n', timeout);

        //+WIND:xx:
        if (buff[0] == '+') {
            count++;
            if (buff[7] == ':') //one digit
            {
                wind = buff[6] - '0';
            } else //two digit
            {
                unsigned char d, u;
                d = ((buff[6] - '0')*10);
                u = (buff[7] - '0');
                wind = d + u;
            }
        }
    } while ((buff[0] != 0)&& (wind != num)&&(count < 5));

    if (buff[0] == 0) //timeout
        return 0;

    return 1;
}

unsigned long remote = 0;

unsigned char ServerStatus(void) {
    unsigned char ret;
    if (eeprom_cfg.TOKEN[0] != 0xFF) {
        strcpy(buff, "GET /api/states/alarm_control_panel.");
        strcat(buff, eeprom_cfg.ID);
        strcat(buff, " HTTP/1.1\r\n");
        strcat(buff, "Host: ");
        strcat(buff, eeprom_cfg.SERVER);
        strcat(buff, ":");
        strcat(buff, eeprom_cfg.PORT);
        strcat(buff, "\r\n");
        strcat(buff, "User-Agent: SPWF01SA11\r\n");
        strcat(buff, "Accept: */*\r\n");
        strcat(buff, "Authorization: Bearer ");
        strcat(buff, eeprom_cfg.TOKEN);
        strcat(buff, "\r\n");
        strcat(buff, "Content-Type: application/json\r\n");
        strcat(buff, "\r\n");

        serial_tx('A');
        serial_clear_buffer();
        serial_tx_str("T+S.HTTPREQ=");
        serial_tx_str(eeprom_cfg.SERVER);
        serial_tx_str(",");
        serial_tx_str(eeprom_cfg.PORT);
        serial_tx_str(",");
        serial_tx_str(utoa2(strlen(buff), itoabuff));
        serial_tx_str("\r");
        serial_tx_str(buff);

        ret = 0;
        buff[0] = 0;
        do {
            serial_rx_str_until(buff, 1024, '\n', TOUT);

            if (buff[0] == '{')//verifica se e JSON
            {
                if (strstr(buff, "\"state\": \"disarmed\"")) {
                    ret = REMOTE_CMD_OFF;
                } else if (strstr(buff, "\"state\": \"armed_away\"")) {
                    ret = REMOTE_CMD_ON;
                } else if (strstr(buff, "\"state\": \"triggered\"")) {
                    ret = REMOTE_CMD_OFF;
                }
            }
            buff[6] = 0;

        } while (buff[0] && strcmp(buff, "OK\r\n") && strcmp(buff, "ERROR:"));

        return ret;
    } else {

        strcpy(strcmd, "T+S.HTTPPOST=");
        strcat(strcmd, eeprom_cfg.SERVER);
        strcat(strcmd, ",/alard.php,id=");
        strcat(strcmd, eeprom_cfg.ID);
        strcat(strcmd, "&cmd=");
        strcat(strcmd, "AL_STATUS");
        strcat(strcmd, ",");
        strcat(strcmd, eeprom_cfg.PORT);

        serial_tx('A');
        serial_clear_buffer();
        serial_tx_str(strcmd);
        serial_tx_str("\r");
        ret = 0;
        buff[0] = 0;
        do {
            serial_rx_str_until(buff, 1024, '\n', TOUT);

            if (buff[0] == '[')//verifica se ¿ comando
            {
                if (strstr(buff, "[Alarme:Off]")) {
                    ret = REMOTE_CMD_OFF;
                } else if (strstr(buff, "[Alarme:On]")) {
                    ret = REMOTE_CMD_ON;
                }
            }
            buff[6] = 0;

        } while (buff[0] && strcmp(buff, "OK\r\n") && strcmp(buff, "ERROR:"));

        return ret;
    }
    return 0;
}

unsigned char FindRemote(int timeout) {
    buff[0] = 0;
    buff[1] = 0;
    buff[2] = 0;
    serial_rx_str_until(buff, 1024, '\n', timeout);

    //if codigo de sensor, come¿ado com # 
    if ((buff[0] == '%')&&(buff[1] == '2')&&(buff[2] == '3')) {
        remote = 0;
        remote += ((unsigned long) ((buff[ 3] > '9') ? (buff[ 3] - 'A' + 10) : (buff[ 3] - '0'))) << 28;
        remote += ((unsigned long) ((buff[ 4] > '9') ? (buff[ 4] - 'A' + 10) : (buff[ 4] - '0'))) << 24;
        remote += ((unsigned long) ((buff[ 5] > '9') ? (buff[ 5] - 'A' + 10) : (buff[ 5] - '0'))) << 20;
        remote += ((unsigned long) ((buff[ 6] > '9') ? (buff[ 6] - 'A' + 10) : (buff[ 6] - '0'))) << 16;
        remote += ((unsigned long) ((buff[ 7] > '9') ? (buff[ 7] - 'A' + 10) : (buff[ 7] - '0'))) << 12;
        remote += ((unsigned long) ((buff[ 8] > '9') ? (buff[ 8] - 'A' + 10) : (buff[ 8] - '0'))) << 8;
        remote += ((unsigned long) ((buff[ 9] > '9') ? (buff[ 9] - 'A' + 10) : (buff[ 9] - '0'))) << 4;
        remote += ((unsigned long) ((buff[10] > '9') ? (buff[10] - 'A' + 10) : (buff[10] - '0')));
        return REMOTE_SENSOR;
    } else if ((buff[0] == '%')&&(buff[1] == '2')&&(buff[2] == '4')) {
        //comando, come¿ado com $
        if (!strcmp(buff + 3, "DISP\r\n")) {
            AL_DISP = 1;
            AL_SETOR = 9;
        } else if (!strcmp(buff + 3, "CMD\r\n")) {

            return ServerStatus();
        }
    }
    return 0;
}

static unsigned char ha_sendstatus(const char *status) {


    strcpy(strcmd, "{\"entity_id\": \"alarm_control_panel.");
    strcat(strcmd, eeprom_cfg.ID);
    strcat(strcmd, "\"}");

    strcpy(buff, "POST /api/services/alarm_control_panel/");
    strcat(buff, status);
    strcat(buff, " HTTP/1.1\r\n");
    strcat(buff, "Host: ");
    strcat(buff, eeprom_cfg.SERVER);
    strcat(buff, ":");
    strcat(buff, eeprom_cfg.PORT);
    strcat(buff, "\r\n");
    strcat(buff, "User-Agent: SPWF01SA11\r\n");
    strcat(buff, "Accept: */*\r\n");
    strcat(buff, "Authorization: Bearer ");
    strcat(buff, eeprom_cfg.TOKEN);
    strcat(buff, "\r\n");
    strcat(buff, "Content-Type: application/json\r\n");
    strcat(buff, "Content-Length: ");
    strcat(buff, utoa2(strlen(strcmd), itoabuff));
    strcat(buff, "\r\n");
    strcat(buff, "\r\n");

    serial_tx('A');
    serial_clear_buffer();
    serial_tx_str("T+S.HTTPREQ=");
    serial_tx_str(eeprom_cfg.SERVER);
    serial_tx_str(",");
    serial_tx_str(eeprom_cfg.PORT);
    serial_tx_str(",");
    serial_tx_str(utoa2(strlen(buff) + strlen(strcmd), itoabuff));
    serial_tx_str("\r");
    serial_tx_str(buff);
    serial_tx_str(strcmd);

    buff[0] = 0;
    do {
        serial_rx_str_until(buff, 1024, '\n', TOUT);
        buff[6] = 0;
    } while (buff[0] && strcmp(buff, "OK\r\n") && strcmp(buff, "ERROR:"));

    return strcmp(buff, "OK\r\n");
}

static unsigned char ha_sendbinarysensor(const char *sensor, int value) {

    strcpy(strcmd, "{\"state\": \"");
    if (value) {
        strcat(strcmd, "on");
    } else {
        strcat(strcmd, "off");
    }
    strcat(strcmd, "\", \"attributes\": {\"friendly_name\": \"");
    strcat(strcmd, sensor);
    strcat(strcmd, "\"}}");

    strcpy(buff, "POST /api/states/binary_sensor.");
    strcat(buff, sensor);
    strcat(buff, " HTTP/1.1\r\n");
    strcat(buff, "Host: ");
    strcat(buff, eeprom_cfg.SERVER);
    strcat(buff, ":");
    strcat(buff, eeprom_cfg.PORT);
    strcat(buff, "\r\n");
    strcat(buff, "User-Agent: SPWF01SA11\r\n");
    strcat(buff, "Accept: */*\r\n");
    strcat(buff, "Authorization: Bearer ");
    strcat(buff, eeprom_cfg.TOKEN);
    strcat(buff, "\r\n");
    strcat(buff, "Content-Type: application/json\r\n");
    strcat(buff, "Content-Length: ");
    strcat(buff, utoa2(strlen(strcmd), itoabuff));
    strcat(buff, "\r\n");
    strcat(buff, "\r\n");

    serial_tx('A');
    serial_clear_buffer();
    serial_tx_str("T+S.HTTPREQ=");
    serial_tx_str(eeprom_cfg.SERVER);
    serial_tx_str(",");
    serial_tx_str(eeprom_cfg.PORT);
    serial_tx_str(",");
    serial_tx_str(utoa2(strlen(buff) + strlen(strcmd), itoabuff));
    serial_tx_str("\r");
    serial_tx_str(buff);
    serial_tx_str(strcmd);

    buff[0] = 0;
    do {
        serial_rx_str_until(buff, 1024, '\n', TOUT);
        buff[6] = 0;
    } while (buff[0] && strcmp(buff, "OK\r\n") && strcmp(buff, "ERROR:"));


    return strcmp(buff, "OK\r\n");

}

static unsigned char ha_sendsensor(const char *sensor, const char *value) {

    strcpy(strcmd, "{\"state\": \"");
    strcat(strcmd, value);
    strcat(strcmd, "\", \"attributes\": {\"friendly_name\": \"");
    strcat(strcmd, sensor);
    strcat(strcmd, "\"}}");

    strcpy(buff, "POST /api/states/sensor.");
    strcat(buff, sensor);
    strcat(buff, " HTTP/1.1\r\n");
    strcat(buff, "Host: ");
    strcat(buff, eeprom_cfg.SERVER);
    strcat(buff, ":");
    strcat(buff, eeprom_cfg.PORT);
    strcat(buff, "\r\n");
    strcat(buff, "User-Agent: SPWF01SA11\r\n");
    strcat(buff, "Accept: */*\r\n");
    strcat(buff, "Authorization: Bearer ");
    strcat(buff, eeprom_cfg.TOKEN);
    strcat(buff, "\r\n");
    strcat(buff, "Content-Type: application/json\r\n");
    strcat(buff, "Content-Length: ");
    strcat(buff, utoa2(strlen(strcmd), itoabuff));
    strcat(buff, "\r\n");
    strcat(buff, "\r\n");

    serial_tx('A');
    serial_clear_buffer();
    serial_tx_str("T+S.HTTPREQ=");
    serial_tx_str(eeprom_cfg.SERVER);
    serial_tx_str(",");
    serial_tx_str(eeprom_cfg.PORT);
    serial_tx_str(",");
    serial_tx_str(utoa2(strlen(buff) + strlen(strcmd), itoabuff));
    serial_tx_str("\r");
    serial_tx_str(buff);
    serial_tx_str(strcmd);

    buff[0] = 0;
    do {
        serial_rx_str_until(buff, 1024, '\n', TOUT);
        buff[6] = 0;
    } while (buff[0] && strcmp(buff, "OK\r\n") && strcmp(buff, "ERROR:"));


    return strcmp(buff, "OK\r\n");

}

static unsigned char ka = 0;

unsigned char SendToServer(const unsigned char cmd, const char * code) {

    if (eeprom_cfg.TOKEN[0] != 0xFF) {
        char sensorb[20];
        char sensor[20];
        char status[20];
        char value = -1;

        status[0] = 0;
        sensorb[0] = 0;
        sensor[0] = 0;
        switch (cmd) {
            case AL_CONFIG:
                return 0;
                break;
            case AL_CADASTRO:
                return 0;
                break;
            case AL_DISP_ON:
                strcpy(status, "alarm_trigger");
                strcpy(sensorb, "al_disp_");
                strcat(sensorb, eeprom_cfg.ID);
                value = 1;
                strcpy(sensor, "al_setor_");
                strcat(sensor, eeprom_cfg.ID);
                break;
            case AL_DISP_OFF:
                strcpy(sensorb, "al_disp_");
                strcat(sensorb, eeprom_cfg.ID);
                value = 0;
                break;
            case AL_KEEP_AL:
                strcpy(sensorb, "al_keepa_");
                strcat(sensorb, eeprom_cfg.ID);
                value = ka;
                ka ^= 1;
                break;
            case AL_WIFI_ON:
                strcpy(status, "alarm_arm_away");
                strcpy(sensorb, "al_on_");
                strcat(sensorb, eeprom_cfg.ID);
                value = 1;
                break;
            case AL_WIFI_OFF:
                strcpy(status, "alarm_disarm");
                strcpy(sensorb, "al_on_");
                strcat(sensorb, eeprom_cfg.ID);
                value = 0;
                break;
            case AL_SETOR_ON:
                strcpy(sensor, "al_setor_");
                strcat(sensor, eeprom_cfg.ID);
                break;
            default:
                return 0;
                break;
        }

        if (status[0]) {
            ha_sendstatus(status);
        }

        if (sensor[0]) {
            ha_sendsensor(sensor, code);
        }

        if (sensorb[0]) {
            return ha_sendbinarysensor(sensorb, value);
        }
    } else {

        strcpy(strcmd, "AT+S.HTTPPOST=");
        strcat(strcmd, eeprom_cfg.SERVER);
        strcat(strcmd, ",/alard.php,id=");
        strcat(strcmd, eeprom_cfg.ID);
        strcat(strcmd, "&cmd=");
        switch (cmd) {
            case AL_CONFIG:
                strcat(strcmd, "AL_CONFIG");
                break;
            case AL_CADASTRO:
                strcat(strcmd, "AL_CADASTRO");
                break;
            case AL_DISP_ON:
                strcat(strcmd, "AL_DISP_ON");
                break;
            case AL_DISP_OFF:
                strcat(strcmd, "AL_DISP_OFF");
                break;
            case AL_WIFI_ON:
                strcat(strcmd, "AL_WIFI_ON");
                break;
            case AL_WIFI_OFF:
                strcat(strcmd, "AL_WIFI_OFF");
                break;
            case AL_KEEP_AL:
                strcat(strcmd, "AL_KEEP_AL");
                break;
            case AL_SETOR_ON:
                strcat(strcmd, "AL_SETOR_ON");
                break;
        }
        if (code) {
            strcat(strcmd, "&code=");
            strcat(strcmd, code);
        }
        strcat(strcmd, ",");
        strcat(strcmd, eeprom_cfg.PORT);
        return SendAT(strcmd, TOUT);
    }
    return 0;
}

static char prog_buff[64];

unsigned char FindConfig(int timeout) {
    unsigned char ret = 0;
    buff[0] = 0;
    buff[1] = 0;
    buff[2] = 0;
    serial_rx_str_until(buff, 1024, '\n', timeout);


    if ((buff[0] == '%')&&(buff[1] == '2')&&(buff[2] == '4')&&(buff[4] == '_')) {
        //comando, come¿ado com $  padr¿o $x:xxxxxx
        buff[strlen(buff) - 2] = 0; //elimina \r \n
        switch (buff[3]) {
            case 'i'://identificador central
            case 'I'://identificador central    
                if (eeprom_cfg.ID[0] == 0xFF) {
                    ret = 1;
                    flash_write_block((unsigned long) &eeprom_cfg.ID, (unsigned char *) (buff + 5));
                }
                break;
            case 's'://endere¿o servidor
            case 'S'://endere¿o servidor    
                if (eeprom_cfg.SERVER[0] == 0xFF) {
                    ret = 1;
                    flash_write_block((unsigned long) &eeprom_cfg.SERVER, (unsigned char *) (buff + 5));
                }
                break;
            case 'p'://porta servidor
            case 'P'://porta servidor    
                if (eeprom_cfg.PORT[0] == 0xFF) {
                    ret = 1;
                    flash_write_block((unsigned long) &eeprom_cfg.PORT, (unsigned char *) (buff + 5));
                }
                break;
            case '1'://parte 1 Token
                if (eeprom_cfg.TOKEN[0] == 0xFF) {
                    ret = 1;
                    strncpy(prog_buff, buff + 5, 32);
                }
                break;
            case '2'://parte 2 Token
                if (eeprom_cfg.TOKEN[0] == 0xFF) {
                    ret = 1;
                    strncpy(prog_buff + 32, buff + 5, 32);
                    flash_write_block((unsigned long) &eeprom_cfg.TOKEN, (unsigned char *) prog_buff);
                }
                break;
            case '3'://parte 3 Token
                if (eeprom_cfg.TOKEN[64] == 0xFF) {
                    ret = 1;
                    strncpy(prog_buff, buff + 5, 32);
                }
                break;
            case '4'://parte 4 Token
                if (eeprom_cfg.TOKEN[64] == 0xFF) {
                    ret = 1;
                    strncpy(prog_buff + 32, buff + 5, 32);
                    flash_write_block(((unsigned long) &eeprom_cfg.TOKEN) + 64L, (unsigned char *) prog_buff);
                }
                break;
            case '5'://parte 3 Token
                if (eeprom_cfg.TOKEN[128] == 0xFF) {
                    ret = 1;
                    strncpy(prog_buff, buff + 5, 32);
                }
                break;
            case '6'://parte 4 Token
                if (eeprom_cfg.TOKEN[128] == 0xFF) {
                    ret = 1;
                    strncpy(prog_buff + 32, buff + 5, 32);
                    flash_write_block(((unsigned long) &eeprom_cfg.TOKEN) + 128L, (unsigned char *) prog_buff);
                }
                break;
            default:
                ret = 0;
                break;
        }

        if (ret) {
            AUX_BUZZER = 1;
            delay_ms(100);
            AUX_BUZZER = 0;
            delay_ms(100);
            AUX_BUZZER = 1;
            delay_ms(100);
            AUX_BUZZER = 0;
        } else {
            AUX_BUZZER = 1;
            delay_ms(500);
            AUX_BUZZER = 0;
        }
    }


    return ret;
}
