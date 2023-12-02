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
    return jsonify(
        {
            'payload': request.json,
        }
    )

@app.route('/users/<gid>', methods=['GET', 'POST'])
def users(gid):
    # return a JSON list of users in a group
    return jsonify([{'user_id': i,'group_id': gid } for i in range(42)])

@app.route('/healthcheck', methods=['GET'])
def healthcheck():
    # return some JSON
    return jsonify({'key': 'healthcheck', 'status': 200})

# @app.route('/healthcheck', methods=['GET'])
# def healthcheck():
#     # return some JSON
#     return jsonify({'key': 'healthcheck', 'status': 200})


if __name__ == "__main__":
    with app.test_request_context():
        app.debug = True
    app.run(debug=True, host='0.0.0.0', port=8000)