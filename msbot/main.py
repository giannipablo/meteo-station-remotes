import os
import telebot
import requests

from tabulate import tabulate


API_KEY = os.getenv('MS_API_KEY')
URL = os.getenv('MS_URL', 'http://127.0.0.1:8000/pull')

bot = telebot.TeleBot(API_KEY)

# URL = 'http://homemeteostation.duckdns.org/current'

vars = {'temperature': 'Temp', 'humidity': 'Humedad', 'wind_speed': 'Viento', 'preasure': 'Presion'}

qry =  {
    "uuid" : "640f9d98660d41d7bbe4edf9887de873",
    "dev_name" : "Ricardo 1"
}

# Handle '/start' and '/help'
@bot.message_handler(commands=['reporte'])
def send_meteovars(message):



    req = requests.post(URL, json=qry).json()

    valores = []
    for k,v in vars.items():
        valores.append([v, req['payload'][k]])
    valores = tabulate(valores, headers=['Variable    ', 'Valor'])
    
    bot.send_message(message.chat.id, valores)

bot.infinity_polling()
