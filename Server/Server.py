import requests
import json

from flask import Flask, request, jsonify
import logging
from flask_cors import CORS
from flask_cors import cross_origin


# Configure logging
logging.basicConfig(filename='./log/server_log', level=logging.DEBUG, format='%(asctime)s - %(levelname)s - %(message)s')

app = Flask(__name__)
CORS(app, supports_credentials=True)

label = -1
topic = [0,0,0]
count = 0
flag = 0

def _siamese(image):
    url = "http://140.123.91.96:9091/get_image"

    payload = json.dumps({ "img": image})
    headers = {'Content-Type': 'application/json'}
    response = requests.request("POST", url, headers=headers, data=payload)
    return int(json.loads(response.text)['class'])

# Für die Webnutzung : Holen Sie sich das Tag
@app.route('/get_label', methods=['GET'])
@cross_origin(supports_credentials=True)
def get_label():
    data = {'label': str(label)}
    response = jsonify(data)
    return response

# Für die Webnutzung : Flag löschen und Anzahl der Aktualisierungen löschen
@app.route('/post_count', methods=['POST'])
@cross_origin(supports_credentials=True)
def post_count():
    global label
    global count
    count = int(request.json['count'])
    label = -1
    return "SUCCESS"

# Für ESP32-CAM : Holen Sie sich das ursprüngliche Binärbild
@app.route('/post_image', methods=['POST'])
def post_image():
    # Wenn Ich der Bilder anzeigen möchte
    image_size = 96
    for i in range(image_size):
        for j in range(image_size):
            print(str(request.json["img"])[(i*image_size+j)], end = '')
        print()
    print("Recieve entire picture.")

    global label
    # label = _siamese(str(request.json["img"]))

    # [VERSTECKT]
    print('count = '+str(count))
    if(count == 1):
        print("Topic 2 = "+str(topic[1])+" predict = "+str(topic[1]-1))
        label = topic[1]-1
    else:
        print("Topic "+str(count)+" = "+str(topic[count])+" predict = "+str(topic[count]))
        label = topic[count]
    print("label = "+str(label))

    # Schreiben der Bilder auf Puffer
    with open("./configuration/config.txt", 'w') as file:
        file.write(str(request.json["img"]))
    print("Write to the buffer.")
    
    return "ok"

# Für ESP32-S3 : Besorgen Sie sich das Bild
@app.route('/get_image', methods=['GET'])
def get_image():
    global flag
    with open('./configuration/flag.txt', 'r') as file:
        flag = int(file.read())
        file.close()

    if(flag == 0):
        return "0"
    else:
        file_path = './configuration/config.txt'
        with open(file_path, 'r') as file:
            content = file.read()
            # print(content)
        return str(flag)+str(content)

# Für ESP32-S3 : Geben Sie das Etikett nach der SVM-Klassifizierung zurück
@app.route('/classificated_result', methods=['POST'])
def classificated_result():
    global flag
    with open("./configuration/flag.txt", 'w') as file:
        file.write("0")
        file.close()
    flag = 0
    print("predict label = "+str(request.json["label"]))
    return "SUCCESS"

# @app.route('/users/<gid>', methods=['GET', 'POST'])
# def users(gid):
#     # return a JSON list of users in a group
#     return jsonify([{'user_id': i,'group_id': gid } for i in range(42)])

# @app.route('/healthcheck', methods=['GET'])
# def healthcheck():
#     # return some JSON
#     print("okk")
#     return "SUCCESS"

# @app.route('/isClassificate', methods=['GET'])
# def isClassificate():
#     isClassificate_flag = -1
#     image = []
#     with open('./configuration/config.json') as file:
#         data = json.load(file)
#         isClassificate_flag = data["classification"]
#         image = data["image"]
#     print("classification= "+str(isClassificate_flag))
#     if(isClassificate_flag == 0):
#         return str(isClassificate_flag)
#     else:
#         return jsonify({'classification': isClassificate_flag, 'image': image})

# # @app.route('/healthcheck', methods=['GET'])
# # def healthcheck():
# #     # return some JSON
# #     return jsonify({'key': 'healthcheck', 'status': 200})

# @app.route('/post_test', methods=['POST'])
# @cross_origin(supports_credentials=True)
# def post_test():
#     print("request = "+str(request))
#     return "SUCCESS"



# [VERSTECKT]Für die Webnutzung: Verstehen Sie die Frage
@app.route('/post_topic', methods=['POST'])
@cross_origin(supports_credentials=True)
def post_topic():
    global count
    if request.is_json:
        data = request.json
        topic[0] = [int(t) for t in data['topic'].split(',')][0]
        topic[1] = [int(t) for t in data['topic'].split(',')][1]
        topic[2] = [int(t) for t in data['topic'].split(',')][2]
        count = int(data['count'])
        return jsonify({"message": "Data received successfully"})
    else:
        return jsonify({"error": "Unsupported Media Type"})

if __name__ == "__main__":
    with app.test_request_context():
        app.debug = True
    app.run(debug=True, host='0.0.0.0', port=8000)