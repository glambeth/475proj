#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <cassert>
#include "ec_ops.h"
using namespace std;

Zp inverseHelper(Zp a, Zp b) {
	Zp b0(b.getValue());
	Zp t;
	uberzahl q;
	Zp x0(uberzahl(0));
	Zp x1(uberzahl(1));
	if (b.getValue() == uberzahl(1)) {
		return 1;
	}
	while (a.getValue() > 1) {
		q = a.getValue()/b.getValue();
		t = b;
		b = Zp(a.getValue() % b.getValue());
		a = t;
		t = x0;
		x0 = Zp(x1.getValue() - q *x0.getValue());
		x1 = t;
	}
	
	if (x1.getValue() < 0) {
		x1 = Zp(x1.getValue() + b0.getValue());
	}
	
	return x1;

}

Zp Zp::inverse() const{
	// Implement the Extended Euclidean Algorithm to return the inverse mod PRIME		\
	
	//b represents the prime for the field.
	Zp b(uberzahl(5));
	//assert(0);
	return inverseHelper(*this, b);
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

	assert(0);
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
	
	Zp test(2);
	cout << test.inverse() << endl;
	
	
}



