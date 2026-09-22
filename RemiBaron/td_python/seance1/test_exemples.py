from exemples import is_prime, crible_eratosthene
import unittest

#Attention, ne pas oublier "test_" avant les fonctions test sinon elles ne sont pas testées

class TestIsPrime(unittest.TestCase):
    def test_basic_prime(self):
        self.assertTrue(is_prime(11))
        self.assertFalse(is_prime(75))
    
    def test_small_num(self):
        self.assertTrue(is_prime(2))
        self.assertTrue(is_prime(3))
        self.assertFalse(is_prime(4))
        
class TestCrible(unittest.TestCase):
    def test_basic_tests(self):
        self.assertEqual(crible_eratosthene(2), [False, False])
        self.assertEqual(crible_eratosthene(10), [False, False, True, True, False, True, False, True, False, False])
        self.assertEqual(crible_eratosthene(100), crible_eratosthene(1000)[:100])
        
    def test_check_big_num(self):
        l = crible_eratosthene(1000)
        assert all(is_prime(i) for i in range(1000) if l[i])
        
    def test_check_precondition(self):
        with self.assertRaises(AssertionError):
            crible_eratosthene(1)
        
        with self.assertRaises(AssertionError):
            crible_eratosthene(-50)
            
if __name__ == "__main__":
    unittest.main()