from http.client import ImproperConnectionState
import os
import unittest
import json
from flaskr import create_app

class SimulationTestCase(unittest.TestCase):
    """This class represents the trivia test case"""
    def setUp(self):
        """Define test variables and initialize app.
            it is run before each test
        """
        self.app = create_app()
        self.client = self.app.test_client     
                
    # in case you want to clean the database after each request
    def tearDown(self):
        """Executed after each test"""
        pass

    def test_get_main_ok(self):
        # print('hello')
        
        res = self.client().get('/') # res is of a type stream
        self.assertEqual(res.status_code, 200)

    def test_get_who_ok(self):
        
        res = self.client().get('/who') # res is of a type stream
        answer = json.loads(res.data) # data is a of type string of characters
        self.assertIn(answer["answer"], ["simulator"])
        # print(answer)
        self.assertEqual(res.status_code, 200)

    def test_get_L_prediction_ok(self):
        
        res = self.client().get('/prediction?action=L') # res is of a type stream
        answer = json.loads(res.data) # data is a of type string of characters
        # print(answer)
        self.assertIn(answer["prediction"], ["L","R","F","B" , "I"])
        self.assertEqual(res.status_code, 200)
    def test_get_R_prediction_ok(self):
        
        res = self.client().get('/prediction?action=R') # res is of a type stream
        answer = json.loads(res.data) # data is a of type string of characters
        # print(answer)
        self.assertIn(answer["prediction"], ["L","R","F","B" , "I"])
        self.assertEqual(res.status_code, 200)
    def test_get_F_prediction_ok(self):
        
        res = self.client().get('/prediction?action=F') # res is of a type stream
        answer = json.loads(res.data) # data is a of type string of characters
        # print(answer)
        self.assertIn(answer["prediction"], ["L","R","F","B" , "I"])
        self.assertEqual(res.status_code, 200)
    def test_get_B_prediction_ok(self):
        
        res = self.client().get('/prediction?action=B') # res is of a type stream
        answer = json.loads(res.data) # data is a of type string of characters
        # print(answer)
        self.assertIn(answer["prediction"], ["L","R","F","B" , "I"])
        self.assertEqual(res.status_code, 200)

    def test_get_idle_prediction_ok(self):      
        res = self.client().get('/prediction?action=I') # res is of a type stream
        answer = json.loads(res.data) # data is a of type string of characters
        # print(answer)
        self.assertIn(answer["prediction"], ["L","R","F","B" , "I"])
        self.assertEqual(res.status_code, 200)
    def test_get_prediction_error(self):
        # print('hello')W
        res = self.client().get('/prediction') # res is of a type stream
        self.assertEqual(res.status_code, 422)


# Make the tests conveniently executable
if __name__ == "__main__":
    unittest.main()
