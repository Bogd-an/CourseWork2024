# CourseWork2024
> з мікроконтролерів та мікропроцесорної техніки
>
> на тему: Програмований пристрій сигналізації

## Структура репозиторію
- [docs](docs)
    - [Пояснююча записка pdf](docs/КР_2024_ПМ11_Погорєлов.pdf)
    - [Презентація pdf](docs/Презентація_КР_2024_ПМ11_Погорєлов.pdf)
    - [кресленики](docs/кресленики) у pdf та сирцеві файли
- [home_assistant](home_assistant)
    - [Py скрипт запуску серверу](home_assistant/server.py)
- [sketch](sketch)
## Запуск

### Software
1. Завантаження даного репозиторію та відкриття папки `CourseWork2024/home_assistant`
2. Інсталяція [Docker](https://www.docker.com/get-started/) 
3. Інсталяція [Python](https://www.python.org/downloads/)
4. Інсталяція необхідних бібліотек 
`pip install paho-mqtt flask`
5. Запуск `python server.py`
    - у консолі запуститься середовище та буде присутній ВАШ_АДРЕС_ЛОКАЛЬНОГО_СЕРВЕРУ
6. Відкритя GUI Home Assistant http://localhost:8123/
    - логін `root`, пароль `toor`


### Hardware
1. Встановлення [Arduino IDE](https://www.arduino.cc/en/software) та додавання 
[cdk для ESP](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html#installing-using-arduino-ide)
    - Також додайте [бібліотеки](sketch/libraries) у `HOME_DIRECTORY/Arduino/libraries/`

2. Збірка деталей згідно [схеми](sketch/diagram.png)

3. Програмування (потрібно вказати свої параметри)
```Arduino
WIFI_SSID     = "ВАША_НАЗВА_МЕРЕЖІ";
WIFI_PASSWORD = "ВАШ_ПАРОЛЬ_МЕРЕЖІ";
server_address= "ВАШ_АДРЕС_ЛОКАЛЬНОГО_СЕРВЕРУ/sensor";
```

### Тестування 
[Wokwi схема](https://wokwi.com/projects/398089119267814401) пояснення на 
[ПЗ 26 сторінка](docs/КР_2024_ПМ11_Погорєлов.pdf)

