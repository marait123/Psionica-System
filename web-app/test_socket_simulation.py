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

    # testing the greetings
    def test_get_L_prediction_ok(self):
        
        res = self.client().get('/simulation') # res is of a type stream
        answer = json.loads(res.data) # data is a of type string of characters
        print(answer)
        self.assertIn(answer["simulation_status"], [False,True])
        self.assertEqual(res.status_code, 200)
    # testing the greetings

# Make the tests conveniently executable
if __name__ == "__main__":
    unittest.main()
