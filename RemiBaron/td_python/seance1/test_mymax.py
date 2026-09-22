from mymax import mymax
import unittest


class TestMyMax(unittest.TestCase):
    def test_mymax_basic(self):
        assert mymax([1, -1, 1, -1, 1, -1]) == 1

    def test_mymax_lengthone(self):
        assert mymax([42]) == 42
    
    def test_mymax_empty_list(self):
        with self.assertRaises(ValueError):
            mymax([])
            
    """def test_fail(self):
        self.assertEqual(mymax([67]), 89) 
        #FAIL: test_fail (__main__.TestMyMax)
        #----------------------------------------------------------------------
        #Traceback (most recent call last):
        #...
        #----------------------------------------------------------------------
        #Ran 4 tests in 0.000s

        #FAILED (failures=1)
        """


if __name__ == "__main__":
    unittest.main()
