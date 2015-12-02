#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <cassert>
#include "ec_ops.h"
using namespace std;


pair<uberzahl, pair<uberzahl, uberzahl> > inverseHelper(uberzahl a, uberzahl b) {
	if (a == "0") {
		return make_pair(b, make_pair(uberzahl(0), uberzahl(1)));
	}
	pair<uberzahl, pair<uberzahl, uberzahl> > p;
	p = inverseHelper((b % a), a);
	return make_pair(p.first, make_pair(p.second.second -
	 				(p.second.first * (b / a)), p.second.first));
}

Zp Zp::inverse() const {
	// Implement the Extended Euclidean Algorithm to return the inverse mod PRIME
	return (inverseHelper(this->value, PRIME).second.first + PRIME) % PRIME;
}


ECpoint ECpoint::operator + (const ECpoint &a) const {
	// Implement  elliptic curve addition 		

	assert(0);
	return ECpoint(true);
}


ECpoint ECpoint::repeatSum(ECpoint p, uberzahl v) const {
	//Find the sum of p+p+...+p (vtimes)		

	assert(0);
	return ECpoint(true);
}

Zp ECsystem::power(Zp val, uberzahl pow) {
	//Find the product of val*val*...*val (pow times)

	
	return 0;
}


uberzahl ECsystem::pointCompress(ECpoint e) {
	//It is the gamma function explained in the assignment.
	//Note: Here return type is mpz_class because the function may
	//map to a value greater than the defined PRIME number (i.e, range of Zp)
	//This function is fully defined.	
	uberzahl compressedPoint = e.x.getValue();
	compressedPoint = compressedPoint<<1;
	
	if(e.infinityPoint) {
		cout<<"Point cannot be compressed as its INF-POINT"<<flush;
		abort();
		}
	else {
		if (e.y.getValue()%2 == 1)
			compressedPoint = compressedPoint + 1;
		}
		//cout<<"For point  "<<e<<"  Compressed point is <<"<<compressedPoint<<"\n";
		return compressedPoint;

}

ECpoint ECsystem::pointDecompress(uberzahl compressedPoint){
	//Implement the delta function for decompressing the compressed point

	assert(0);
	return ECpoint(true);
}


pair<pair<Zp,Zp>,uberzahl> ECsystem::encrypt(ECpoint publicKey, uberzahl privateKey,Zp plaintext0,Zp plaintext1){
	// You must implement elliptic curve encryption
	//  Do not generate a random key. Use the private key that is passed from the main function

	assert(0);
	return make_pair(make_pair(0,0),0);
}


pair<Zp,Zp> ECsystem::decrypt(pair<pair<Zp,Zp>, uberzahl> ciphertext){
	// Implement EC Decryption

	assert(0);
	return make_pair(0,0);
}


/*
 * main: Compute a pair of public key and private key
 *       Generate plaintext (m1, m2)
 *       Encrypt plaintext using elliptic curve encryption
 *       Decrypt ciphertext using elliptic curve decryption
 *       Should get the original plaintext
 *       Don't change anything in main.  We will use this to 
 *       evaluate the correctness of your program.
 */


int main(void){
	/*
	srand(time(0));
	ECsystem ec;
	unsigned long incrementVal;	
	pair <ECpoint, uberzahl> keys = ec.generateKeys();
	
	
	Zp plaintext0(MESSAGE0);
	Zp plaintext1(MESSAGE1);
	ECpoint publicKey = keys.first;
	cout<<"Public key is: "<<publicKey<<"\n";
	
	cout<<"Enter offset value for sender's private key"<<endl;
	cin>>incrementVal;
	uberzahl privateKey = XB + incrementVal;
	
	pair<pair<Zp,Zp>, uberzahl> ciphertext = ec.encrypt(publicKey, privateKey, plaintext0,plaintext1);	
	cout<<"Encrypted ciphertext is: ("<<ciphertext.first.first<<", "<<ciphertext.first.second<<", "<<ciphertext.second<<")\n";
	pair<Zp,Zp> plaintext_out = ec.decrypt(ciphertext);
	
	cout << "Original plaintext is: (" << plaintext0 << ", " << plaintext1 << ")\n";
	cout << "Decrypted plaintext: (" << plaintext_out.first << ", " << plaintext_out.second << ")\n";


	if(plaintext0 == plaintext_out.first && plaintext1 == plaintext_out.second)
		cout << "Correct!" << endl;
	else
		cout << "Plaintext different from original plaintext." << endl;	
			
	return 1;

	*/
	
<<<<<<< HEAD
	Zp test(1);
=======
	Zp test(2);
>>>>>>> 43c06934fe7e4590ca2919e057ad3c8d29e462bf
	cout << test.inverse() << endl;

	Zp test1(2);
	cout << test1.inverse() << endl;

	Zp test2(3);
	cout << test2.inverse() << endl;

	Zp test3(4);
	cout << test3.inverse() << endl;

	Zp test4(5);
	cout << test4.inverse() << endl;

	Zp test5(-1);
	cout << test5.inverse() << endl;

	Zp test6(6);
	cout << test6.inverse() << endl;
}



