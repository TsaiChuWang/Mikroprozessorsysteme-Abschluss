import json

from flask import Flask, request, jsonify
import logging

# Configure logging
logging.basicConfig(filename='./log/server_log', level=logging.DEBUG, format='%(asctime)s - %(levelname)s - %(message)s')

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def root():
    # spit back whatever was posted + the full env 
    return jsonify(
        {
            'request.json': request.json,
            'request.values': request.values,
            'env': json.loads(json.dumps(request.__dict__, sort_keys=True, default=str))
        }
    )

@app.route('/post', methods=['GET', 'POST'])
def post():
    if not request.json:
        return 'No JSON payload! Expecting POST!'
    # return the literal POST-ed payload
    # print(request.json[img])
    for i in range(96):
        for j in range(96):
            print(str(request.json["img"])[(i*96+j)], end = '')
        print()
    return "ok"
@app.route('/get', methods=['GET'])
def get():
    # return some JSON
    print("ok")
    return jsonify({'key': 'healthcheck', 'status': 200})

@app.route('/users/<gid>', methods=['GET', 'POST'])
def users(gid):
    # return a JSON list of users in a group
    return jsonify([{'user_id': i,'group_id': gid } for i in range(42)])

@app.route('/healthcheck', methods=['GET'])
def healthcheck():
    # return some JSON
    return jsonify({'key': 'healthcheck', 'status': 200})

@app.route('/isClassificate', methods=['GET'])
def isClassificate():
    isClassificate_flag = -1
    image = []
    with open('./configuration/config.json') as file:
        data = json.load(file)
        isClassificate_flag = data["classification"]
        image = data["image"]
    print("classification= "+str(isClassificate_flag))
    if(isClassificate_flag == 0):
        return str(isClassificate_flag)
    else:
        return jsonify({'classification': isClassificate_flag, 'image': image})

# @app.route('/healthcheck', methods=['GET'])
# def healthcheck():
#     # return some JSON
#     return jsonify({'key': 'healthcheck', 'status': 200})

@app.route('/post_test', methods=['POST'])
def post_test():
    # return a JSON list of users in a group
    print("request = "+str(request))
    return 0

if __name__ == "__main__":
    with app.test_request_context():
        app.debug = True
    app.run(debug=True, host='0.0.0.0', port=8000)