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
	return Zp(inverseHelper(this->value, PRIME).second.first + PRIME);
}


ECpoint ECpoint::operator + (const ECpoint &a) const {
	// Implement  elliptic curve addition
	if (!(*this == a) && !(this->x == a.x)) {
		//uberzahl slope = (this->y.getValue() - a.y) / (this->x.getValue() - a.x);
		uberzahl slope = (this->y.getValue() - a.y.getValue())*(this->x.getValue() - a.x.getValue());
		Zp calcSlope(slope);
		slope = calcSlope.inverse().getValue();
		uberzahl slopeSquared = slope * slope;

		uberzahl xR = slopeSquared - this->x.getValue() - a.x.getValue();
		uberzahl yR = -this->y.getValue() + slope*(this->x.getValue() - xR);

		Zp xNew(xR);
		Zp yNew(yR);
		return ECpoint(xNew, yNew);
	}
	else if ((*this == a) && (uberzahl(2)*this->y.getValue() != "0")) {
		uberzahl xPSquared = this->x.getValue() * this->x.getValue();
		uberzahl slope = ((uberzahl(3) * xPSquared) - A) * (uberzahl(2) * this->y.getValue());
		Zp calcSlope(slope);
		slope = calcSlope.inverse().getValue();
		uberzahl slopeSquared = slope * slope;

		uberzahl xR = slopeSquared - uberzahl(2)*this->x.getValue();
		uberzahl yR = -this->y.getValue() + slope*(this->x.getValue() - xR);

		Zp xNew(xR);
		Zp yNew(yR);
		return ECpoint(xNew, yNew);
	}
	else {
		return ECpoint(true);
	}
}

ECpoint repeatSumHelper(ECpoint sum, ECpoint p, uberzahl v) {
	if (v == "0") {
		return sum;
	}
	else {
		return repeatSumHelper(sum + p, p, v-"1");
	}
}

ECpoint ECpoint::repeatSum(ECpoint p, uberzahl v) const {
	//Find the sum of p+p+...+p (vtimes)		
	return repeatSumHelper(ECpoint(Zp(uberzahl(0)), Zp(uberzahl(0))), p, v);
}

uberzahl powerHelper(uberzahl base, uberzahl power) {
	if (power == "1") {
		return base;
	}
	else {
		if ((power % uberzahl(2)) == "0") {
			uberzahl base0 = powerHelper(base, power / uberzahl(2));
			base0 = base0 * base0;
			if (base0 >= PRIME) {
				return base0 % PRIME;
			}
			else {
				return base0;
			}
		}
		else {
			uberzahl base1 = powerHelper(base, (power - uberzahl(1)) / uberzahl(2));
			base1 = base1 * base1;
			uberzahl ans = base * base1;
			if (ans >= PRIME) {
				return ans % PRIME;
			}
			else {
				return ans;
			}
		}
	}
}

Zp ECsystem::power(Zp val, uberzahl pow) {
	//Find the product of val*val*...*val (pow times)
	return Zp(powerHelper(val.getValue(), pow));
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
	uberzahl x;
	if (compressedPoint % 2 == 1) {
		x = compressedPoint - "1";
	}
	x = x / "2";
	Zp xR(x);

	uberzahl xRcubed = this->power(x, uberzahl(3)).getValue() % PRIME;
	uberzahl yEven = (xRcubed + (A * x) + B) % PRIME;
	uberzahl exponent = PRIME + uberzahl(1) / uberzahl(4);
	uberzahl yOdd = this->power(yEven, exponent).getValue() % PRIME;
	Zp yR;

	if (compressedPoint.bit(1) % 2 == 0) {
		yR = Zp(yEven);
	}
	else {
		yR = Zp(PRIME - yOdd);
	}

	return ECpoint(xR, yR);
}


pair<pair<Zp,Zp>,uberzahl> ECsystem::encrypt(ECpoint publicKey, uberzahl privateKey,Zp plaintext0,Zp plaintext1){
	// You must implement elliptic curve encryption
	//  Do not generate a random key. Use the private key that is passed from the main function


	//step 1: generate a random integer X, 0 <= X <= N - 1
	uberzahl x = random(uberzahl(0), uberzahl(ORDER_STR - 1));
	
	//step 2: compute Q = X*G = (xq, yq) and R = X*P = (X*Y)*G = (xr,yr) using repeated squaring
	ECpoint q = privateKey * ECpoint(GX,GY);
	ECpoint r = privateKey * publicKey;
	
	//step 3: set C0 = M0*xr mod p1 and C1 = M1*yr modp1 and C2 = y(Q)
	Zp c0 = (plaintext0 * r.x).getValue() % PRIME_STR;
	Zp c1 = (plaintext1 * r.y).getValue() % PRIME_STR;
	uberzahl c2 = this->pointCompress(q);
	
	//return C = (c0, c1, c2)
	
	return make_pair(make_pair(c0,c1),c2);
}


pair<Zp,Zp> ECsystem::decrypt(pair<pair<Zp,Zp>, uberzahl> ciphertext){
	// Implement EC Decryption

	//step 1: Compute R = Y *lambda(C2) 
	ECpoint r = this->privateKey * this->pointDecompress(ciphertext.second);

	
	//step 2: Set Mo = C0 *Xr^-1 modp and M1 = C1*yr^-1 mod p
	Zp m0 = (ciphertext.first.first * r.x.inverse()).getValue() % PRIME_STR;
	Zp m1 = (ciphertext.first.second * r.y.inverse()).getValue() % PRIME_STR;
	
	//return MoM1
	return make_pair(m0,m1);
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

	
	
	// set PRIME_STR to 5
	Zp test(1);
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

	// set PRIME_STR to 53
	Zp val0(uberzahl(3));
	uberzahl expon = uberzahl(37);
	ECsystem test;
	cout << "The result is: " << test.power(val0, expon) << endl;
	cout << "It should be 32 (in mod 53) or 450283905890997363 in PRIME" << endl;

	Zp val1(uberzahl(7));
	uberzahl expon1 = uberzahl(9);
	ECsystem test1;
	cout << "The result is: " << test1.power(val1, expon1) << endl;
	cout << "It should be 43 (in mod 53) or 40353607 in PRIME" << endl;

	Zp val2(uberzahl(11));
	uberzahl expon2 = uberzahl(15);
	ECsystem test2;
	cout << "The result is: " << test2.power(val2, expon2) << endl;
	cout << "It should be 38 (in mod 53) or 4177248169415651 in PRIME" << endl;

	*/

}



