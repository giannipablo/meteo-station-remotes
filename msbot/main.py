import os
import telebot
import requests

from tabulate import tabulate


MS_API_KEY = os.getenv('MS_API_KEY')

bot = telebot.TeleBot(MS_API_KEY)

URL = 'http://homemeteostation.duckdns.org/current'

vars = {'temperature': 'Temp', 'humidity': 'Humedad', 'wind_speed': 'Viento'}

# Handle '/start' and '/help'
@bot.message_handler(commands=['reporte'])
def send_meteovars(message):

    req = requests.get(URL).json()
    
    valores = []
    for k,v in vars.iteritems():
        valores.append([v, req['payload'][k]])
    valores = tabulate(valores, headers=['Variable', 'Valor'])
    
    bot.send_message(message.chat.id, valores)

bot.infinity_polling()
