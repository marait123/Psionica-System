from distutils.core import setup
from logging import debug
from flask import Flask, json, request, jsonify, abort, render_template

from flask_cors import CORS

Page_count = 2


def create_app(test_config=None):
    app = Flask(__name__)

    # cors = CORS(app, resources={r"/api/*": {"origins": "*"}})
    cors = CORS(app, resources={"*": {"origins": "*"}})

    @app.after_request
    def after_request(response):
        response.headers.add('Access-Control-Allow-Headers',
                             'Content-Type,Authorization,true')
        response.headers.add('Access-Control-Allow-Methods',
                             'GET,PATCH,POST,DELETE,OPTIONS')
        return response

    @app.route('/', methods=['GET'])
    def index():
        return render_template('index.html')
    
    @app.route('/hello', methods=['GET'])
    def hellohello():
        print("hello")
        return "<h1>hello hello baby</h1>"

    @app.route('/hi', methods=['GET'])
    def hihi():
        print("hi")
        return "<h1>hi hi baby</h1>"

    @app.errorhandler(422)
    def unprocessable(error):
        return jsonify({
            "success": False,
            "error": 422,
        }), 422

    @app.errorhandler(404)
    def unprocessable(error):
        return jsonify({
            "message": "resource not found",
            "success": False,
            "error": 404,
        }), 404

    return app
if __name__ == "__main__":
    print("hello all")
    # app.run(debug=True)