from flask import Flask, request, render_template
import requests, os
app = Flask(__name__)

@app.route('/')
def my_form():
   #return render_template('index.html')

   url = "http://supervisor/addons/self/options/config"
   #url = "http://192.168.1.91:8123/api/config"
   headers = {
       "Authorization": "Bearer " + os.environ.get('SUPERVISOR_TOKEN'),
       "content-type": "application/json",
   }

   response = requests.get(url, headers=headers)
   print(response.text)
   return render_template('index.html', rt=response.text)


if __name__ == '__main__':
    app.debug = True
    app.run(host="0.0.0.0")
