How to use Universal Telegram Bot:
1. Create bot from BotFather on Telegram
    * send /newbot to request new bot from BotFather
    * Create your bot name
    * Create username for your bot (must end in 'bot')
    * Finish, your bot is ready to set up
2. Find your ID Client from IDBOT on Telegram
    * send /getid for get your ID from IDBOT
    * Finish, now you know your ID
3. Using library UniversalTelegramBot version 1.3.0
    * You can use library from folder library
    * At Arduino IDE, click Sketch -> Include Library -> Add .ZIP Library -> search your downloaded library -> Click Open
    * OR At Arduino IDE, click Library Manager, and search UniversalTelegramBot by Brian Lough, make sure the version is 1.3.0 -> Click INSTALL
4. Using library ArduinoJson version 6.15.2 or 6.20.1
    * At Arduino IDE, click Library Manager, and search ArduinoJson by Benoit Blanchon, make sure the version is 6.20.1 -> Click INSTALL
    * If error from this version maybe you can change the version by 6.15.2
5. Make sure your board type and your port at Device Manager
    * Make sure copy this json at your Arduino IDE -> File -> Preferences -> Additional boards manager URLs -> and click OK
      https://dl.espressif.com/dl/package_esp32_index.json
      http://arduino.esp8266.com/stable/package_esp8266com_index.json
      http://digistump.com/package_digistump_index.json
      http://drazzy.com/package_drazzy.com_index.json
6. Running sketch_mar7a.ino
7. Bot will send message first if WIFI connected