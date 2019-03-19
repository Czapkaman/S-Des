#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

constexpr int8_t Sbox1[16] = { 1, 0, 3, 2, 3, 2, 1, 0, 0, 2, 1, 3, 3, 1, 3, 2 };
constexpr int8_t Sbox2[16] = { 0, 1, 2, 3, 2, 0, 1, 3, 3, 0, 1, 0, 2, 1, 0, 3 };

bool Read(vector<char> &Vec, const char* File_name)
{
	ifstream RawText(File_name, ifstream::in);

	if (RawText.is_open())
	{
		char temp = 0;
		int i = 0;
		RawText.seekg(0, RawText.end);
		i = RawText.tellg();
		Vec.reserve(i);
		RawText.seekg(0, RawText.beg);
		while (i--)
		{
			temp = RawText.get();
			if (temp > 64 && temp < 91) temp = (temp + 32 - 19) % 26 + 97;
			if ((temp < 97 || temp > 122) && !(temp > 47 && temp <58))continue;
			Vec.emplace_back(temp);
		}
		RawText.close();
		return 0;
	}
	else
	{
		cout << "Error, cannot find \"" << File_name << "\"" << endl;
		return 1;
	}
}

void PrintBitsIn2x2BitsBlock(vector<bool> &text)
{
	for (unsigned j = 0; j < text.size(); j+=4)
	{
		cout << text[j] << text[j + 1] << " ";
	}
	cout << endl;
	for (unsigned j = 2; j < text.size(); j += 4)
	{
		cout << text[j] << text[j + 1] << " ";
	}
	cout << endl;
}

void PrintBitsInHex(vector<bool> &text)
{
	int temp = 0;
	for (unsigned j = 0; j < text.size(); j += 4)
	{
		if (j % 8 == 0 && j > 1)
		{
			cout << " ";
		}
		temp = text[0 + j] * 8 + text[1 + j] * 4 + text[2 + j] * 2 + text[3 + j] * 1;
		if (temp < 10)
		{
			cout << temp;
		}
		else
		{
			cout << static_cast<char>(97 + temp - 10);
		}
	}
	cout << endl;
}

void PrintVecBool(vector<bool> &text)
{
	for (auto l : text) cout << l;
	cout << endl;
}

void HexToBits(vector<char> &Text, vector<bool> &Bits)
{
	for (auto L : Text)
	{
		if (L < 58)
		{
			for (int i = 3; i >= 0; i--)
			{
				Bits.emplace_back((L >> i) & 1);
			}
		}
		else
		{
			Bits.emplace_back(1);
			L += 1;
			for (int i = 2; i >= 0; i--)
			{
				Bits.emplace_back((L >> i) & 1);
			}
		}
	}
	if(Bits.size() % 8 != 0)
		Bits.insert(Bits.end(), { 0, 0, 0, 0 });
}

inline void ClearVecIfPossible(vector<bool> &Vec) 
{ if (Vec.size() != 0) { Vec.clear(); } }

inline vector<bool> P10(vector<bool> &bits, int b0, int b1, int b2, int b3, int b4, int b5, int b6, int b7, int b8, int b9) 
{ return  { bits[b0], bits[b1], bits[b2], bits[b3], bits[b4], bits[b5], bits[b6], bits[b7], bits[b8], bits[b9] }; }

inline vector<bool> P4(vector<bool> &bits, int b0, int b1, int b2, int b3) 
{ return { bits[b0], bits[b1], bits[b2], bits[b3] }; }

inline vector<bool> P10w8(vector<bool> &bits) 
{ return { bits[5], bits[2], bits[6], bits[3], bits[7], bits[4], bits[9], bits[8] }; }

inline vector<bool> P4w8(vector<bool> &bits) 
{ return { bits[3], bits[0], bits[1], bits[2], bits[1], bits[2], bits[3], bits[0] }; }

vector<bool> P8(vector<bool> &bits, int b0, int b1, int b2, int b3, int b4, int b5, int b6, int b7)
{
	vector<bool> temp;
	temp.reserve(8);
	for (unsigned int j = 0; j < bits.size(); j += 8)
	{
		temp.insert(temp.end(), { bits[b0 + j], bits[b1 + j], bits[b2 + j], bits[b3 + j], bits[b4 + j], bits[b5 + j], bits[b6 + j], bits[b7 + j] });
	}
	return temp;
}

vector<bool> operator^ (vector<bool> lhs, vector<bool> rhs)
{
	if (lhs.size() != rhs.size())
		return { 0 };
	for (unsigned int i = 0; i < lhs.size(); i++)
	{
		lhs[i] = lhs[i] ^ rhs[i];
	}
	return lhs;
}

vector<bool> ValueOfSBoxs(vector<bool> VecBool)
{
	vector<bool> temp;
	if (Sbox1[(VecBool[0] * 2 + VecBool[3]) * 4 + (VecBool[1] * 2 + VecBool[2])] == 3)
	{
		temp.insert(temp.end(), { 1, 1 });
	}
	else if (Sbox1[(VecBool[0] * 2 + VecBool[3]) * 4 + (VecBool[1] * 2 + VecBool[2])] == 2)
	{
		temp.insert(temp.end(), { 1, 0 });
	}
	else if (Sbox1[(VecBool[0] * 2 + VecBool[3]) * 4 + (VecBool[1] * 2 + VecBool[2])] == 1)
	{
		temp.insert(temp.end(), { 0, 1 });
	}
	else if (Sbox1[(VecBool[0] * 2 + VecBool[3]) * 4 + (VecBool[1] * 2 + VecBool[2])] == 0)
	{
		temp.insert(temp.end(), { 0, 0 });
	}

	if (Sbox2[(VecBool[4] * 2 + VecBool[7]) * 4 + (VecBool[5] * 2 + VecBool[6])] == 3)
	{
		temp.insert(temp.end(), { 1, 1 });
	}
	else if (Sbox2[(VecBool[4] * 2 + VecBool[7]) * 4 + (VecBool[5] * 2 + VecBool[6])] == 2)
	{
		temp.insert(temp.end(), { 1, 0 });
	}
	else if (Sbox2[(VecBool[4] * 2 + VecBool[7]) * 4 + (VecBool[5] * 2 + VecBool[6])] == 1)
	{
		temp.insert(temp.end(), { 0, 1 });
	}
	else if (Sbox2[(VecBool[4] * 2 + VecBool[7]) * 4 + (VecBool[5] * 2 + VecBool[6])] == 0)
	{
		temp.insert(temp.end(), { 0, 0 });
	}
	return temp;
}

vector<bool> Round(const bool Which_round, vector<bool> &text, vector<bool> &Key, vector<bool> &SboxsValues)
{
	vector<bool> lhs, rhs, rhs2, Sboxs, CryptedText;
	for (unsigned int i = 0; i < text.size(); i += 8)
	{
		lhs.insert(lhs.begin(), { text[0 + i], text[1 + i], text[2 + i], text[3 + i] });
		rhs2.insert(rhs2.begin(), { text[4 + i], text[5 + i], text[6 + i], text[7 + i] });

		if (Which_round) { lhs.swap(rhs2); }

		rhs = P4w8(rhs2) ^ Key;

		Sboxs = ValueOfSBoxs(rhs);
		SboxsValues.insert(SboxsValues.end(), Sboxs.begin(), Sboxs.end());
		lhs = lhs ^ P4(Sboxs, 1, 3, 2, 0);

		CryptedText.insert(CryptedText.end(), lhs.begin(), lhs.end());
		CryptedText.insert(CryptedText.end(), rhs2.begin(), rhs2.end());

		ClearVecIfPossible(lhs);
		ClearVecIfPossible(rhs);
		ClearVecIfPossible(rhs2);
		ClearVecIfPossible(Sboxs);
	}
	return CryptedText;
}

vector<bool> Crypting(vector<bool> &text, vector<bool> &k_1, vector<bool> &k_2)
{
	vector<bool> temp_k_1, temp_k_2, CryptedText_r1, CryptedText_r2;
	temp_k_1.reserve(text.size());
	temp_k_2.reserve(text.size());

	CryptedText_r1 = Round(0, text, k_1, temp_k_1);
	PrintBitsInHex(CryptedText_r1);
	PrintBitsIn2x2BitsBlock(temp_k_1);

	CryptedText_r2 = Round(1, CryptedText_r1, k_2, temp_k_2);
	PrintBitsInHex(CryptedText_r2);
	PrintBitsIn2x2BitsBlock(temp_k_2);

	return CryptedText_r2;
}

vector<bool> Decrypting(vector<bool> &text, vector<bool> &k_1, vector<bool> &k_2)
{
	vector<bool> temp_k_1, temp_k_2, CryptedText_r1, CryptedText_r2;

	CryptedText_r1 = Round(0, text, k_2, temp_k_1);
	CryptedText_r2 = Round(1, CryptedText_r1, k_1, temp_k_2);

	return CryptedText_r2;
}

int main()
{
	vector<char> Text;
	vector<bool> BitsOfText, Keyp, Key1, Key2;
	string prompt;
	bool check = 0;
	if (Read(Text, "tekst.txt"))
	{
		return 1;
	}
	else
	{
		cout << "Warning! Hex values will be without 0x prefix!\n";
		BitsOfText.reserve(Text.size() * 4);
		HexToBits(Text, BitsOfText);
		PrintBitsInHex(BitsOfText);

		cout << "Please give 10 bits key: ";
		Keyp.reserve(10);
		while (cin >> prompt)
		{
			if (prompt.length() == 1)
			{
				if (prompt[0] != '0' && prompt[0] != '1')
				{
					cout << "Invalid input" << prompt[0] << "!\n";
					cout << "Please give me key in bits\n";
					continue;
				}
				else if (prompt[0] == '0')
				{
					Keyp.emplace_back(0);
				}
				else
				{
					Keyp.emplace_back(1);
				}
			}
			else if (prompt.length() == 10 - Keyp.size())
			{
				check = 1;
				for (auto b : prompt)
				{
					if (b != '0' && b != '1')
					{
						cout << "Invalid input" << b << "!\n";
						cout << "Please give me key in bits\n";
						check = 0;
						break;
					}
				}
				if (check)
				{
					for (auto b : prompt)
					{
						if (b == '0')
						{
							Keyp.emplace_back(0);
						}
						else
						{
							Keyp.emplace_back(1);
						}
					}
				}
			}
			else
			{
				cout << "Invalid input \"" << prompt << "\"!\n";
				cout << "Please give me key bit by bit or " << 10 - Keyp.size() << " bits at once!\n";
			}
			if (Keyp.size() == 10) break;
		}

		Keyp = P10(Keyp, 2, 4, 1, 6, 3, 9, 0, 8, 7, 5);

		Keyp = P10(Keyp, 1, 2, 3, 4, 0, 6, 7, 8, 9, 5);
		Key1 = P10w8(Keyp);

		Keyp = P10(Keyp, 2, 3, 4, 0, 1, 7, 8, 9, 5, 6);
		Key2 = P10w8(Keyp);

		BitsOfText = P8(BitsOfText, 1, 5, 2, 0, 3, 7, 4, 6);
		BitsOfText = Crypting(BitsOfText, Key1, Key2);
		BitsOfText = P8(BitsOfText, 3, 0, 2, 4, 6, 1, 7, 5);
		PrintBitsInHex(BitsOfText);

		BitsOfText = P8(BitsOfText, 1, 5, 2, 0, 3, 7, 4, 6);
		BitsOfText = Decrypting(BitsOfText, Key1, Key2);
		BitsOfText = P8(BitsOfText, 3, 0, 2, 4, 6, 1, 7, 5);
		PrintBitsInHex(BitsOfText);
	}
	return 0;
}
