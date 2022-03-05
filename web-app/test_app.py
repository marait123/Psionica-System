from http.client import ImproperConnectionState
import os
import unittest
import json
from flask_sqlalchemy import SQLAlchemy
from models import Greeting, setup_db, db
from flaskr import create_app
# TODO:
# make sure you create a database named hello_test in psql
database_name = "hello_test"
database_path = 'postgresql://postgres:123456@localhost:5432/{}'.format(
    database_name)
greetings = [{'lang': 'en', 'greeting': 'hello'},
             {'lang': 'es', 'greeting': 'Hola'},
             {'lang': 'ar', 'greeting': 'مرحبا'},
             {'lang': 'ru', 'greeting': 'Привет'},
             {'lang': 'fi', 'greeting': 'Hei'},
             {'lang': 'he', 'greeting': 'שלום'},
             {'lang': 'ja', 'greeting': 'こんにちは'}]


class GreetingTestCase(unittest.TestCase):
    """This class represents the trivia test case"""

    def setUp(self):
        """Define test variables and initialize app.
            it is run before each test
        """
        self.app = create_app()
        self.client = self.app.test_client
        self.database_path = database_path
        setup_db(self.app, self.database_path)

        # binds the app to the current context
        # each flask app has a context that includes all of the apps configuration like database, password, etc
        with self.app.app_context():
            # self.db = SQLAlchemy()
            self.db = db
            self.db.init_app(self.app)
            # clean the database beforehand
            self.db.drop_all()
            self.db.session.commit()

            # create all tables
            self.db.create_all()
            self.db.session.commit()

        with self.app.app_context():
            # insert the categories
            for greeting in greetings:
                cat = Greeting(**greeting)
                self.db.session.add(cat)
                self.db.session.commit()
                
    # in case you want to clean the database after each request
    def tearDown(self):
        """Executed after each test"""
        with self.app.app_context():
            # clean the database beforehand
            self.db.drop_all()
            self.db.session.commit()
        pass

    # testing the greetings
    def test_get_greetings_ok(self):
        # print('hello')
        print('hello test_get_greetings_ok')
        
        res = self.client().get('/greetings') # res is of a type stream
        # print(res.data) # res.data is a of type string of characters
        new_greeetings = json.loads(res.data) # data is a of type string of characters
        # print(new_greeetings)
        self.assertEqual(res.status_code, 200)

    # test getting one greeting
    def test_one_greeting_ok(self):
        print('hello test_one_greeting')
        res = self.client().get(f'/greetings/en')
        # print("res " , res)
        new_greeting = json.loads(res.data)       
        # print("new_greeting ",new_greeting)
        self.assertEqual(res.status_code, 200)
         
        self.assertEqual(new_greeting["greeting"]['greeting'], greetings[0]["greeting"])
        
    
    # test getting one greeting
    def test_one_greeting_404(self):
        print('hello test_one_greeting_404')
        res = self.client().get(f'/greetings/notfound')
        # print("res " , res)
        new_greeting = json.loads(res.data)       
        # print("new_greeting ",new_greeting)
        self.assertEqual(res.status_code, 404)
         
        # self.assertEqual(new_greeting["greeting"]['greeting'], greetings[0]["greeting"])
        
    
    # test getting one greeting
    def test_create_greeting_ok(self):
        print('hello test_create_greeting_ok')
        res = self.client().post(f'/greetings',json=dict(lang="french", greeting="bonjour"))
        print("res " , res)
        print("res.data " , res.data)
        new_greeting = json.loads(res.data)       
        # print("new_greeting ",new_greeting)
        self.assertEqual(res.status_code, 201)
         
        # self.assertEqual(new_greeting["greeting"]['greeting'], greetings[0]["greeting"])
        
    #TODO: implement a test case to test getting one beautiful greeting
    def test_beautiful_greeting(self):
        print('hello test_beautiful_greeting')
     
        self.assertEqual(200, 200)
        
# Make the tests conveniently executable
if __name__ == "__main__":
    unittest.main()
