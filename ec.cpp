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
		uberzahl slopeRise = (this->y.getValue() - a.y.getValue());
		uberzahl slopeRun = (this->x.getValue() - a.x.getValue());
		Zp runInverse(slopeRun);
		slopeRun = runInverse.inverse().getValue();
		uberzahl slope = slopeRise * slopeRun;
		Zp slopeSquared(slope * slope);

		uberzahl xR = slopeSquared.getValue() - this->x.getValue() - a.x.getValue();
		uberzahl yR = -this->y.getValue() + slope*(this->x.getValue() - xR);

		Zp xNew(xR);
		Zp yNew(yR);
		return ECpoint(xNew, yNew);
	}
	else if ((*this == a) && (Zp(uberzahl(2)*this->y.getValue()).getValue() != "0")) {
		uberzahl xPSquared = this->x.getValue() * this->x.getValue();
		uberzahl slopeRise = Zp((uberzahl(3) * xPSquared) + A).getValue();
		uberzahl slopeRun = Zp(uberzahl(2) * this->y.getValue()).getValue();
		Zp calcSlope(slopeRun);
		slopeRun = calcSlope.inverse().getValue();
		uberzahl slope = slopeRise * slopeRun;
		Zp slopeSquared(slope * slope);

		uberzahl xR = slopeSquared.getValue() - uberzahl(2)*this->x.getValue();
		uberzahl yRNegated = -(this->y.getValue());
		uberzahl yR = yRNegated + slope*(this->x.getValue() - xR);

		Zp xNew(xR);
		Zp yNew(yR);
		return ECpoint(xNew, yNew);
	}
	else {
		return ECpoint(true);
	}
}

ECpoint repeatSumHelper(ECpoint p, uberzahl v) {
	if (v == "0") {
		return ECpoint(uberzahl(0), uberzahl(0));
	}
	else if (v == "1") {
		return p;
	}
	else if ((v % "2") == "1") {
		return p + repeatSumHelper(p, v - "1");
		//return repeatSumHelper(sum + p, p, v-"1");
	}
	else {
		return repeatSumHelper(p + p, v / "2");
	}
}

ECpoint ECpoint::repeatSum(ECpoint p, uberzahl v) const {
	//Find the sum of p+p+...+p (vtimes)		
	return ECpoint(repeatSumHelper(p, v));
}

uberzahl powerHelper(uberzahl base, uberzahl power) {
	if (power == "1") {
		return base;
	}
	else {
		if (power % "2" == "0") {
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
			uberzahl base1 = powerHelper(base, (power - "1") / "2");
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
	uberzahl x = compressedPoint>>1;
	Zp xR(x);

	uberzahl xRcubed = this->power(x, "3").getValue();
	uberzahl ySquared = Zp(xRcubed + (A * x) + B).getValue();
	uberzahl exponent = Zp((PRIME + "1") / "4").getValue();
	uberzahl yRootOne = this->power(ySquared, exponent).getValue();
	uberzahl yRootTwo = Zp(PRIME - yRootOne).getValue();
	Zp yR;

	if (compressedPoint.bit(0) == yRootOne.bit(0)) {
		yR = Zp(yRootOne);
	}
	else {
		yR = Zp(yRootTwo);
	}

	return ECpoint(xR, yR);
}


pair<pair<Zp,Zp>,uberzahl> ECsystem::encrypt(ECpoint publicKey, uberzahl privateKey,Zp plaintext0,Zp plaintext1){
	// You must implement elliptic curve encryption
	//  Do not generate a random key. Use the private key that is passed from the main function

	//step 2: compute Q = X*G = (xq, yq) and R = X*P = (X*Y)*G = (xr,yr) using repeated squaring (repeatSum)
	ECpoint Q = privateKey * ECpoint(GX,GY);
	ECpoint R = privateKey * publicKey;
	
	//step 3: set C0 = M0*xr mod p1 and C1 = M1*yr modp1 and C2 = y(Q)
	Zp c0 = (plaintext0.getValue() * R.x.getValue());
	Zp c1 = (plaintext1.getValue() * R.y.getValue());
	uberzahl c2 = this->pointCompress(Q);
	
	return make_pair(make_pair(c0,c1),c2);
}


pair<Zp,Zp> ECsystem::decrypt(pair<pair<Zp,Zp>, uberzahl> ciphertext){
	// Implement EC Decryption

	//step 1: Compute R = Y *lambda(C2) 
	ECpoint decompressed = this->pointDecompress(ciphertext.second);
	ECpoint R = XA * decompressed;
	
	//step 2: Set Mo = C0 *Xr^-1 modp and M1 = C1*yr^-1 mod p
	Zp m0 = ciphertext.first.first.getValue() * R.x.inverse().getValue();
	Zp m1 = ciphertext.first.second.getValue() * R.y.inverse().getValue();
	
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
	
	srand(time(0));
	ECsystem ec;
	unsigned long incrementVal;	
	cout << "HERE 0" << endl;
	pair <ECpoint, uberzahl> keys = ec.generateKeys();
	cout << "HERE 1" << endl;
	
	Zp plaintext0(MESSAGE0);
	Zp plaintext1(MESSAGE1);
	cout << "HERE 2" << endl;
	ECpoint publicKey = keys.first;
	cout<<"Public key is: "<<publicKey<<"\n";
	
	cout<<"Enter offset value for sender's private key"<<endl;
	cin>>incrementVal;
	uberzahl privateKey = XB + incrementVal;
	cout << "HERE 3" << endl;
	
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

}



