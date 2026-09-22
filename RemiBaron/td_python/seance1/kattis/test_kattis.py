from exercices import get_r2, cetiri, thanos, digits, basic_prog, magic_division, server_needed, clock_display, bits_equalizer, memory_match
import unittest

class TestExercices(unittest.TestCase):
    def test_samples_ex1(self):
        self.assertEqual(get_r2(11, 15), 19)
        self.assertEqual(get_r2(4, 3), 2)
        
    def test_samples_ex2(self):
        self.assertEqual(cetiri(4,6,8), 10)
        self.assertEqual(cetiri(10, 1, 4), 7)
        
    def test_sample_ex3(self):
        self.assertEqual(thanos(3,[[1,3,9],[2,2,16],[5,2,11]]), [3,4,2])
        
    def test_sample_ex4(self):
        self.assertEqual(digits([42,5]), [3,2])
        
    def test_samples_ex5(self):
        self.assertEqual(basic_prog(7, 1, [1,2,3,4,5,6,7]), 7)
        self.assertEqual(basic_prog(7, 2, [1,2,3,4,5,6,7]), "Smaller")
        self.assertEqual(basic_prog(7, 3, [1,2,3,4,5,6,7]), 2)
        self.assertEqual(basic_prog(7, 4, [1,2,3,4,5,6,7]), 28)
        self.assertEqual(basic_prog(7, 5, [1,2,3,4,5,6,7]), 12)
        self.assertEqual(basic_prog(10, 6, [7,4,11,37,14,22,40,17,11,3]), "helloworld")
        self.assertEqual(basic_prog(3,7,[1,0,2]), "Cyclic")

    def test_samples_ex6(self):
        self.assertEqual(magic_division(92746237, 100000), "927.46237")
        self.assertEqual(magic_division(100000, 100), "1000")
        self.assertEqual(magic_division(1234500, 10000), "123.45")
        self.assertEqual(magic_division(1,10), "0.1")

    def test_samples_ex7(self):
        self.assertEqual(server_needed(2,1,[0,1000]), 1)
        self.assertEqual(server_needed(3,2,[1000, 1010, 1999]), 2)

    def test_samples_ex8(self):
        print("")
        clock_display("16:47")
        clock_display("23:59")
        clock_display("00:08")  
        print("END")
        #tests printed

    def test_samples_ex9(self):
        self.assertEqual(bits_equalizer("01??00", "001010"), 3)
        self.assertEqual(bits_equalizer("01", "10"), 1)
        self.assertEqual(bits_equalizer("110001", "000000"), -1)
        self.assertEqual(bits_equalizer("01??00", "001100"), 3)
        self.assertEqual(bits_equalizer("1", "0"), -1)
        self.assertEqual(bits_equalizer("0", "1"), 1)
        self.assertEqual(bits_equalizer("?", "0"), 1)
        self.assertEqual(bits_equalizer("001010", "001010"), 0)
        self.assertEqual(bits_equalizer("001010", "001001"), 1)
        self.assertEqual(bits_equalizer("000000", "001010"), 2)
        
    def test_samples_ex10(self):
        self.assertEqual(memory_match(8,5,[(1,3), (2,6), (6,3), (7,5), (2,7)],[("earth", "sun"), ("mars", "sun"), ("sun", "sun"), ("earth", "moon"), ("mars", "earth")]), 3)
        self.assertEqual(memory_match(10,6,[(1,2),(9,10),(8,7),(1,8),(4,10),(9,6)],[("moon", "earth"), ("venus", "sun"), ("moon", "venus"), ("moon", "moon"), ("sun", "sun"), ("venus", "mars")]), 3)
        self.assertEqual(memory_match(8,2,[(1,3), (2,6)],[("moon", "earth"), ("sun", "earth")]), 1)

if __name__ == "__main__":
    unittest.main()