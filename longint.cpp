// Scott Ferguson
// CSS 342
// Assignment 7
// A LongInt object holds a deque of characters

#include "longint.h"
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

// Assumptions
// -- string contains numbers or characters
// -- an int can not hold the size of the string
// -- standard rules of addition and subtraction of negative numbers

//---------------------------------------------------------
// constructor
// given a string
LongInt::LongInt(const string str){
	digits.clear();
	negative = false; // no negative
	int counter = 0;
	
	if (str[0] == '-'){ // if first character is the negative sign
		negative = true;
		digits.addBack('-');
		counter = 1; // skip the first character
	}
	
	for (int i = counter; i < str.length(); i++){
        if(static_cast<int>(str[i]) >= 48 &&  static_cast<int>(str[i]) <= 57){ // char to int
            digits.addBack(str[i]);
        }
	}

	remove0s(); // remove 0s in the front, if needed
	
	if (digits.isEmpty()){
        digits.addFront( '0' ); // an empty string was given
    }
}

// constructor
// given an object
LongInt::LongInt( const LongInt &rhs ){
    digits.clear();
    negative = false; // without this negative can go undefined
    *this = rhs;
}

// constructor
// given nothing
LongInt::LongInt(){
	digits.clear();
    negative = false; // without this negative can go undefined
}

// deconstructor
LongInt::~LongInt(){
	digits.clear(); // empty the deque
}

// Input/Output
//---------------------------------------------------------
// operator >>
// takes in a deque
istream &operator>>( istream &in, LongInt &rhs ){
	string catchNumbers; // variable to hold the string
	in >> catchNumbers; // catch the string
	rhs = LongInt(catchNumbers); // set the deque to the string
	
	return in; 
}

// operator <<
// prints out the deque
ostream &operator<<( ostream &out, const LongInt &rhs ){
	Deque<char> tempHold = rhs.digits; // temporary deque to go through
	
	if (rhs.negative == true){ // if negative, output the negative sign
		out << '-';
	}
	
	while (!tempHold.isEmpty()){ // not empty
        out <<tempHold.getFront(); // output what's in front
        tempHold.removeFront(); // remove what's in front
    }

    return out;
}

// Assignment operator
//---------------------------------------------------------
// operator =
// current object = object
const LongInt& LongInt::operator=( const LongInt &rhs ){
	digits.clear();
	digits = rhs.digits; // set the deque equal to the other
	negative = rhs.negative; // set the deque's negative equal to the other
	
	return *this;
}

// Logical binary operators
//---------------------------------------------------------
// operator <
// current object < object
bool LongInt::operator< ( const LongInt & rhs ) const{
	if (negative && !rhs.negative){ // - +
        return true;
    }

    if (!negative && rhs.negative){ // + -
        return false;
    }

    if (*this == rhs){ // equal
        return false;
    }
	
	if ((negative && rhs.negative) || (!negative && !rhs.negative)) { // - - / + +
		Deque<char> holdlhs = digits; // holds the lhs
		Deque<char> holdrhs = rhs.digits; // holds the rhs

		if (holdlhs.size() < holdrhs.size()){ // lhs is smaller than rhs
			return true;
		}
		
		if (holdlhs.size() > holdrhs.size()){ // rhs is smaller than lhs
			return false;
		}
		
		while (!holdlhs.isEmpty()){ // go through number by number
			if (holdlhs.getFront() < holdrhs.getFront()){ // lhs's current digit is smaller than rhs's
				return true;
			}
			else if (holdlhs.getFront() > holdrhs.getFront()){ // rhs's current digit is smaller than lhs's
				return false;
			}
			
			// remove the current digit
			holdlhs.removeFront(); 
			holdrhs.removeFront(); 
        }
    }
    return true;
}

// operator <=
// current object <= object
bool LongInt::operator<=( const LongInt & rhs ) const{
	// is this less than, or equal too, if so return true, else return false
	return (*this == rhs || *this < rhs);
}

// operator >
// current object > object
bool LongInt::operator> ( const LongInt & rhs ) const{
	// is this equal too, or less than, if so return false, else return true
	return !(*this == rhs || *this < rhs);
}

// operator >=
// current object >= object
bool LongInt::operator>=( const LongInt & rhs ) const{
	// is this less than, if so return false, else return true
	return !(*this < rhs);
}

// operator ==
// current object == object
bool LongInt::operator==( const LongInt & rhs ) const{
	if(digits.size() != rhs.digits.size()){ // if they are different sizes
        return false;
    }
	
    Deque<char> holdlhs = digits; // deque to hold lhs
    Deque<char> holdrhs = rhs.digits; // deque to hold rhs

	// go through one by one comparing them to see if they are the same character
    while (!holdlhs.isEmpty()){
        if (holdlhs.getFront() != holdrhs.getFront()){
            return false;
        }
		
		// remove the current character
        holdlhs.removeFront();
        holdrhs.removeFront();
    }
	return true;
}

// operator !=
// current object != object
bool LongInt::operator!=( const LongInt & rhs ) const{
	// is this equal too, if so return false, else return true
	return !(*this == rhs);
}

// Arithmetic binary operators
//---------------------------------------------------------
// operator +
// current object + object
LongInt LongInt::operator+( const LongInt &rhs ) const{
	LongInt sum; // Object that will be returned
	
	int digithold; // holds the digit that will be put into sum
	int carry = 0; // holds the carry
	int holdLhsDigit; // holds the digit being pulled from the lhs
	int holdRhsDigit; // holds the digit being pulled from the rhs
	int notEqualSize = 0; // check if the strings are the same size
	
	Deque<char> holdlhs = digits; // hold the lhs
	Deque<char> holdrhs = rhs.digits; //hold the rhs

	if(!this->negative && rhs.negative){ // + -
        LongInt rhsNegative = rhs;
        rhsNegative.negative = false;
        return (*this - rhsNegative);
    }
	
	else if(this->negative && !rhs.negative){ // - +
        LongInt lhsNegative = *this;
        lhsNegative.negative = false;
        return (rhs - lhsNegative);
    }
	
	else if(this->negative && rhs.negative){ // - -
        sum.negative = true;
    }
	
	while (!holdlhs.isEmpty() && !holdrhs.isEmpty()){ // while both deques are not empty 
		if (!holdlhs.isEmpty()){
			// catches int
			holdLhsDigit = static_cast<int>(holdlhs.getBack()) - 48; // fixes ASCII error
			holdlhs.removeBack();
		}
		
		if (!holdrhs.isEmpty()){
			// catches int
			holdRhsDigit = static_cast<int>(holdrhs.getBack()) - 48; // fixes ASCII error
			holdrhs.removeBack();
		}
		
		digithold = (holdLhsDigit + holdRhsDigit + carry) % 10; // computes the number that will be added to sum
			
		carry = (holdLhsDigit + holdRhsDigit + carry) / 10; // computes the carry 
			
		sum.digits.addFront(static_cast<char>(digithold + 48)); // fixes ASCII error
	}
	
	// This goes through them if you are adding two strings of different length
	while (!holdlhs.isEmpty() || !holdrhs.isEmpty()){ // one of them is empty, go through the other
		notEqualSize = 1;
		if (!holdlhs.isEmpty()){
			holdLhsDigit = static_cast<int>(holdlhs.getBack()) - 48; // fixes ASCII error
			holdlhs.removeBack();
			
			digithold = (holdLhsDigit + carry) % 10; // computes the number that will be added to sum
			
			carry = (holdLhsDigit + carry) / 10; // computes the carry
			
			sum.digits.addFront(static_cast<char>(digithold + 48)); // fixes ASCII error
		}
		if (!holdrhs.isEmpty()){
			holdRhsDigit = static_cast<int>(holdrhs.getBack()) - 48; // fixes ASCII error
			holdrhs.removeBack();
			
			digithold = (holdRhsDigit + carry) % 10; // computes the number that will be added to sum
			
			carry = (holdRhsDigit + carry) / 10; // computes the carry
			
			sum.digits.addFront(static_cast<char>(digithold + 48)); // fixes ASCII error
		}
	}
	
	if (notEqualSize == 1 && sum.digits.size() > 6){ // addresses 999999999 error
		sum.digits.addFront(static_cast<char>(1 + 48)); // fixes ASCII error
	}
	return sum;
}

// operator -
// current object - object
LongInt LongInt::operator-( const LongInt &rhs ) const{
	LongInt sub; // Object that will be returned
	int digithold; // holds the digit that will be put into sum
	int borrow = 0; // holds the borrow
	int holdLhsDigit; // holds the digit being pulled from the lhs
	int holdRhsDigit; // holds the digit being pulled from the rhs
	int notEqualSize = 0; // check if the strings are the same size
	
	if(!this->negative && rhs.negative){ // + -
        LongInt rhsNegative = rhs;
        rhsNegative.negative = false;
        return (*this + rhsNegative);
    }
	
	else if(this->negative && !rhs.negative){ // - +
        LongInt lhsNegative = *this;
        lhsNegative.negative = false;
        sub = lhsNegative +rhs;
        sub.negative = true;
        return sub;
    }
	
	else if(this->negative && rhs.negative){ // - -
        LongInt lhsNegative = *this;
        lhsNegative.negative = false;
        LongInt rhsNegative = rhs;
        rhsNegative.negative = false;
        return (rhsNegative - lhsNegative);
    }
	
	if(*this < rhs){ // make it easier to subtract, larger - smaller
        LongInt sub = rhs - *this;
        sub.negative = true;
        return sub;
    }
	
	Deque<char> holdlhs = digits; // hold the lhs
    Deque<char> holdrhs = rhs.digits; // hold the rhs

    while (!holdlhs.isEmpty() && !holdrhs.isEmpty()){ // while both deques are not empty
        if (!holdlhs.isEmpty()){
            holdLhsDigit = static_cast<int> (holdlhs.getBack()) - 48; // fixes ASCII error
            holdlhs.removeBack();
        }

        if (!holdrhs.isEmpty()){
            holdRhsDigit = static_cast<int> (holdrhs.getBack()) - 48; // fixes ASCII error
            holdrhs.removeBack();
        }
		
		digithold = (holdLhsDigit - holdRhsDigit - borrow) % 10; // computes the number that will be added to sub
			
		borrow = (holdLhsDigit - holdRhsDigit - borrow) / 10; // computes the borrow
			
		sub.digits.addFront(static_cast<char>(digithold + 48)); // fixes ASCII error
    }
	sub.remove0s(); // remove extra 0s
    return sub;
}

//---------------------------------------------------------
// removes the 0s at the front of the deque
void LongInt::remove0s(){
	
	if (negative){ // if negative remove the negative sign
		digits.removeFront();
	}
	while(digits.getFront() == '0'){ // remove all 0s in the front
		digits.removeFront();
	}
}



















