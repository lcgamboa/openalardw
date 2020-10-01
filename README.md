

# Openalardw - Open Firmware para central de alarme Alard Max WiFi

## Introdução

Este é um firmware alternativo para ser utilizado na central de alarme Alard Max WiFi da marca ECP.
Este firmware é distribuído como open source sobre a licença GNU GPL sem nenhuma garantia de funcionamento ou de uso, use por sua conta e risco, o autor não se responsabiliza por nenhum dano que possa ocorrer ao seu equipamento ou ocasionado pelo mau funcionamento do mesmo pela utilização do firmware.

O autor não tem nenhuma ligação com a ECP fabricante do equipamento, é apenas proprietário de um central Alard Max WiFi que parou de funcionar com o desligamento do serviço de nuvem da ECP e escreveu o firmware para poder utilizar o equipamento para uso pessoal. O firmware é disponibilizado com o intuito de ajudar outros proprietários da mesma central.

## Características

A central funcionava originalmente conectada a um serviço de nuvem (cloud), onde era possível através de um aplicativo de celular controlar o funcionamento da mesma. Com o fim do serviço de nuvem (e aparente falência da empresa), não é mais possível controlar a central remotamente ou configurar novas centrais. As centrais já configuradas continuarão funcionando com comandos por controle remoto ou chave A/D desde que se mantenham ligadas a bateria. Se a alimentação for retirada a central perde as configurações e não funcionará mais. 


**A atualização para este firmware alternativo é irreversível, não é possível reverter para o firmware original!**

O firmware alternativo não é igual ao original e tem como características:

- Configuração totalmente salva na central (256 sensores e 256 controles)
- Configuração de sensores e controles sem necessidade de aplicativos
- Modo offline (sem rede) e Online(com Rede)
- Modo online para acesso a um servidor local utilizando script PHP ou instância do Home Assistant
- Suporte a repetidores de RF (esp32 utilizando um receptor rf 433MHz por exemplo)

Em relação ao firmware original não foi implementado:

- Discadora DTMF (Não tenho linha telefônica para testar e não utilizo essa função) 
- Conexão em servidor na nuvem (Para segurança só foi implementado suporte para conexão em servidores locais)
- PGMs


## Gravação

Para gravar o firmware na central é necessário conhecimento básico de eletrônica para soldar um conector na placa e um gravador de PIC. 
```
Conector PICKit3  <->   Central
- 1 MCLR --------------- MC
- 2 VCC  --------------- VCC
- 3 GND  --------------- GND
- 4 PGD  --------------- DT
- 5 PGC  --------------- CK
- 6 NC   
```

Pontos de solda na placa:
![im1](docs/im1.jpg?raw=true)

Fios soldados na placa:
![im2](docs/im2.jpg?raw=true)

Conector e fios com cola quente:
![im3](docs/im3.jpg?raw=true)

Conector instalado:
![im4](docs/im4.jpg?raw=true)

## Operação

### Apagar memória

Segurando o botão ENTER da placa por alguns segundos seguidos as configurações vão sendo apagadas:
- por 5  segundos: Apaga configurações da rede WIFI (e entra no modo de configuração WIFI)
- por 10 segundos: Apaga configurações da rede WIFI e configurações do servidor
- por 15 segundos: Apaga configurações da rede WIFI, configurações do servidor e controles cadastrados
- por 20 segundos: Apaga configurações da rede WIFI, configurações do servidor, controles e sensores cadastrados

Cada vez que um item é apagado os LEDs piscam e o buzzer é acionado.


### Configuração sensores e controles (Offline)

Pressionando e soltando o botão ENTER da placa até ouvir o buzzer (menos de 5 segundos) a central entra no modo de programação (LED programação acesso).
A programação é iniciada no setor 1 para sensores (LED 1 acesso), pressionando e soltando o botão ENTER novamente o setor é alterado para o próximo. Depois dos 8 setores está o modo de programação de controle de armar e de desarmar a central. Mais um ENTER depois do último modo e a central volta para o modo de funcionamento normal (LED programação apagado). 

Indicação dos LEDs no modo de gravação:
- LED 1 acesso - gravação de sensor no setor 1
- LED 2 acesso - gravação de sensor no setor 2
- LED 3 acesso - gravação de sensor no setor 3
- LED 4 acesso - gravação de sensor no setor 4
- LED 5 acesso - gravação de sensor no setor 5
- LED 6 acesso - gravação de sensor no setor 6
- LED 7 acesso - gravação de sensor no setor 7
- LED 8 acesso - gravação de sensor no setor 8
- LED 1 e 2 acessos - gravação de botão do controle de armar do alarme
- LED 1, 2 e 3 acessos - gravação de botão do controle de desarmar o alarme

Obs: Não grave o mesmo botão para armar e desarmar o alarme

Nesta central os controles e sensores não tem diferença, pode se usar um botão de controle como sensor e um sensor como botão de armar e desarmar a central (não recomendo).

Como o sinal de RF dos controles (e sensores) não são codificados (usam o formato Learning code) são muito fáceis de serem clonados (com um arduino e un par de receptor e transmissor de RF pode ser feito em minutos [Referência sobre clonagem](https://acturcato.wordpress.com/2014/01/14/clonagem-de-controle-remoto-rf-learning-code-ht6p20b-com-arduino/)) não é seguro utilizar o controle para ligar e desligar a central. Usar o servidor local por website ou aplicativo é bem mais seguro. Uma outra opção e utilizar um módulo de controle remoto com codificação Rolling Code ligado na entrada A/D da central.



### Configuração para conexão servidor local

Para programar o servidor pressione o botão ENTER por 5 segundos até os LEDs piscarem e acionar o buzzer e o solte.

Neste modo a central cria uma rede chamada ECP_MAX que pode ser acessada com a senha ECP12.

Conectado nesta rede, digite o endereço http://captiveportal.net/output.cgi em um navegador.

Formulário output.cgi:

![output.cgi](docs/output.png?raw=true)

Neste formulário devem ser enviadas as informações de configurações. Depois de enviada a informação o buzzer toca um beep curto para OK e um beep longo para erro. As informações só podem ser gravadas uma única vez, depois de gravadas devem ser apagadas para poderem ser configuradas novamente.

O firmware suporta duas configurações de servidor listadas em sequência: 

### Configuração para acesso servidor PHP

Inicialmente utilizei um script PHP (arquivos na pasta PHP) para receber os dados da central, o script deve estar em um servidor local com suporte a PHP e um banco de dados (mysql ou mariadb). Este script é só serve de base para se desenvolver uma aplicação para controle da central, não está pronto para uso direto. 

Para configurar a central para acessar o script alard.php entre com as seguintes informações no formulário output.cgi 

Identificador da central:

**$i_alardw01**

Endereço ou ip da central:

**$s_192.168.0.20**

Porta do servidor:

**$p_80**

Depois de enviadas essas informações, prossiga para configuração do WIFI.



### Configuração para acesso do Home Assistant

O Home Assistant é um sistema open source de automação residencial. Possui acesso por interface WEB ou app para celular. Normalmente o Home assitant é instalado localmente em um raspberry pi ou computador. 

Para configurar a central para acessar o Home assistant entre com as seguintes informações no formulário output.cgi 

Identificador da central:

**$i_alardw01**

Endereço ou ip da central:

**$s_192.168.0.20**

Porta do servidor:

**$p_8123**


Para a central se comunicar com o Home Assistant é ncessário a criação de um Token de acesso de longa duração. O Token gerado tem o formato abaixo:

eyJeaXAiOiJKV1QiLCJhbGciO1JIUzI1NiJwsuyJpc3MiOiJkZtQyMTc1i2JhYTQ0YTd3ejc2MWRiZDt1N2JmZTy5YSIsImlhdCI6MTYwMDt0MzU4OSwiZXhwIjoxOTE1OTKfsTf5fQ.40IzNI-jimxUbgXjSEjESt6OHe65dJsSVfNozzoJqLb


Para envio para central o TOKEN tem que ser dividido em pedaços de 32 caractéres e enviao em 6 partes pelo output.cgi, conforme formato abaixo:

$1_eyJeaXAiOiJKV1QiLCJhbGciO1JIUzI1
$2_NiJwsuyJpc3MiOiJkZtQyMTc1i2JhYTQ
$3_0YTd3ejc2MWRiZDt1N2JmZTy5YSIsIml
$4_hdCI6MTYwMDt0MzU4OSwiZXhwIjoxOTE
$5_1OTKfsTf5fQ.40IzNI-jimxUbgXjSEjE
$6_St6OHe65dJsSVfNozzoJqLb


Depois de enviadas essas informações, prossiga para configuração do WIFI.


### Configuração do servidor Home Assistant

No Home Assitant é necessário criar uma instância de **alarm_control_panel** manual para fazer o controle da central.

A tela do controle da central no Home Assitant é vista na figura abaixo (no app do celular é a mesma interface)
![ha_pc](docs/ha_pc.png?raw=true)



Para configuração é necessário adicionar no arquivo /config/configuration.yaml a configuração de uma (ou mais se for mais de uma central) instância do **alarm_control_panel** com o **name** igual ao id definido na central. Troque o ip 192.168.0.20 pelo ip da sua central na rede.

```
shell_command:
  alardw_cmd: "curl -X GET http://192.168.0.20/output.cgi?text={{ data }}&submit=Submit"


alarm_control_panel:
  - platform: manual
    name: alardw01
#    code: '1234'
    code_arm_required: 0
    arming_time: 2
    delay_time: 1
    trigger_time: 180
    disarmed:
      trigger_time: 0
```

Para gerar notificações no aplicativo, as automações abaixos podem ser adicionadas no arquivo /config/automations.yaml

```
- id: '03'
  alias: Send notification when alarm triggered
  description: ''
  trigger:
  - platform: state
    entity_id: alarm_control_panel.alardw01
    to: triggered
  action:
  - service: notify.notify
    data:
      message: ALARM! The alarm has been triggered

- id: '04'
  alias: Send notification when alarm is Disarmed
  description: ''
  trigger:
  - platform: state
    entity_id: alarm_control_panel.alardw01
    to: disarmed
  action:
  - service: notify.notify
    data:
      message: ALARM! The alarm is Disarmed at {{ states('sensor.date_time') }}
  - service: shell_command.alardw_cmd
    data:
      data: '%24CMD'        

- id: '05'
  alias: Send notification when alarm is Armed 
  description: ''
  trigger:
  - platform: state
    entity_id: alarm_control_panel.alardw01
    to: armed_away
  - platform: state
    entity_id: alarm_control_panel.alardw01
    to: armed_home    
  action:
  - service: notify.notify
    data:
      message: ALARM! The alarm is armed {{ states('sensor.date_time')}}
  - service: shell_command.alardw_cmd
    data:
      data: '%24CMD'  

```

Esta não é a melhor forma de integrar o alarme ao Home Assistant, mas é funcional e atende as necessidades do autor.
Para um produto era interessante o próprio Home Assistant configurar o endereço da central. Mas na versão inicial isso não foi implementado.


### Configuração do WIFI

Conectado na rede ECP_MAX, digite o endereço http://captiveportal.net em um navegador.

Formulário de configuração da rede WIFI:
![wifi](docs/wifi.png?raw=true)

Os dados a serem preenchidos:

- miniAP PassKey: **anonymous**
- SSID: **nome_da_sua_rede_wifi**
- PSK: **senha_da_sua_rede_wifi**
- IP Address: **vazio**  endereço IP fixo, não necessário se o DHCP estiver On
- Net Mask: **vazio** máscara IP fixo, não necessário se o DHCP estiver On
- GW Address: **vazio** gateway IP fixo, não necessário se o DHCP estiver On
- DNS Address: **vazio** DNS IP fixo, não necessário se o DHCP estiver On
- on/off DNS/DHCP: **ON**
- Auth Type: **Open System**
- Choose Auth: **Wpa & Wpa2 Personal**
- Choose Mode: **Station**

Depois de preencher clique no botão GO e depois de OK na mensagem de popup.
Agaurde o LED Config apagar e pressione o botão ENTER da placa para finalizar a configuração e reiniciar a central.

## Hardware da central

A central utiliza um microcontrolador PIC18F46J50 e um módulo wifi SPWF01SA11, as folhas de dados se encontram dentro do diretório datasheets do projeto.

## Melhoramentos

Qualquer melhoramento é bem vindo. Quem quiser contribuir fique a vontade para ajudar a melhorar o projeto. 


