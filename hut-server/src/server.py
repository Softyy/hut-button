from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/',methods=['GET'])
def test():
    return 'Server Up and Running'

@app.route('/hut', methods=['POST'])
def ph():
    data = request.data
    return jsonify(data)