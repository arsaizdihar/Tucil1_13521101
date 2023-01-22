#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <chrono>
using namespace std;

string inputs[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
char ops[] = {'*', '/', '+', '-'};

void get_input(double arr[4])
{
  string raw[4];
  while (true)
  {
    cout << "Pilih opsi untuk masukkan angka:\n"
         << "1. Input manual\n2. Dipilih secara random\n";
    cout << "Masukkan angka 1 atau 2: ";
    int chosen;
    cin >> chosen;
    if (chosen < 1 || chosen > 2)
    {
      cout << "Pilihan tidak valid. Silahkan coba lagi.\n";
      continue;
    }

    if (chosen == 1)
    {
      while (true)
      {
        bool input_valid = true;
        cout << "Masukkan 4 angka/huruf yang terdiri dari (A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K) dipisah dengan spasi:\n";
        for (int i = 0; i < 4; i++)
        {
          cin >> raw[i];
        }
        for (int i = 0; i < 4; i++)
        {
          bool valid = false;
          for (int j = 0; !valid && j < 13; j++)
          {
            if (raw[i] == inputs[j])
            {
              arr[i] = (double)(j + 1);
              valid = true;
            }
          }
          if (!valid)
          {
            cout << "Masukkan tidak valid. Silahkan coba lagi\n";
            input_valid = false;
            break;
          }
        }
        if (input_valid)
          break;
      }
    }
    else
    {
      for (int i = 0; i < 4; i++)
      {
        arr[i] = rand() % 13 + 1;
      }
    }
    printf("Angka yang terpilih: %.0lf, %.0lf, %.0lf, %.0lf\n", arr[0], arr[1], arr[2], arr[3]);
    break;
  }
}

double count_by_op(double a, double b, char op)
{
  switch (op)
  {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  default:
    return a / b;
  }
}

void try_every_paranthesis(vector<string> *answers, double nums[4], char ops[3])
{
  int paranthesis[5][2][2] = {
      {{0, 1}, {2, 3}},
      {{0, 1}, {0, 2}},
      {{1, 2}, {0, 2}},
      {{1, 2}, {1, 3}},
      {{2, 3}, {1, 3}}};

  for (int i = 0; i < 5; i++)
  {
    int *par1 = paranthesis[i][0];
    int *par2 = paranthesis[i][1];
    int ops_idx[3];
    ops_idx[0] = par1[1] - 1;
    if (par1[1] == par2[1])
    {
      ops_idx[1] = par2[0];
    }
    else
    {
      ops_idx[1] = par2[1] - 1;
    }
    ops_idx[2] = 3 - ops_idx[0] - ops_idx[1];

    double res1 = count_by_op(nums[par1[0]], nums[par1[1]], ops[ops_idx[0]]);
    double next1[3];
    bool dec1 = false;
    bool dec2 = false;
    for (int i = 0; i < 3; i++)
    {
      if (i < par1[0])
        next1[i] = nums[i];
      else if (i < par1[1])
        next1[i] = res1;
      else
        next1[i] = nums[i + 1];
    }
    if (par2[0] >= par1[1])
    {
      par2[0]--;
      dec1 = true;
    }
    if (par2[1] >= par1[1])
    {
      par2[1]--;
      dec2 = true;
    }
    double res2 = count_by_op(next1[par2[0]], next1[par2[1]], ops[ops_idx[1]]);
    double next2[2];
    for (int i = 0; i < 2; i++)
    {
      if (i < par2[0])
      {
        next2[i] = next1[i];
      }
      else if (i < par2[1])
      {
        next2[i] = res2;
      }
      else
      {
        next2[i] = next1[i + 1];
      }
    }
    if (dec1)
      par2[0]++;
    if (dec2)
      par2[1]++;
    double res3 = count_by_op(next2[0], next2[1], ops[ops_idx[2]]);
    if (res3 == 24)
    {
      string ans = "";
      for (int i = 0; i < 4; i++)
      {
        if (i == par1[0])
        {
          ans += "(";
        }
        if (i == par2[0])
        {
          ans += "(";
        }
        ans += to_string((int)nums[i]);
        if (i == par1[1])
        {
          ans += ")";
        }
        if (i == par2[1])
        {
          ans += ")";
        }
        if (i < 3)
        {
          ans += " " + string(1, ops[i]) + " ";
        }
      }
      answers->push_back(ans);
    }
  }
}

void show_answers(double nums[4])
{
  auto start = chrono::high_resolution_clock::now();
  vector<array<int, 4>> possibilities;
  vector<array<char, 3>> poss_ops;
  vector<string> answers;

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (i == j)
        continue;
      for (int k = 0; k < 4; k++)
      {
        if (k == i || k == j)
          continue;
        for (int l = 0; l < 4; l++)
        {
          if (l == i || l == j || l == k)
            continue;
          array<int, 4> arr = {i, j, k, l};

          // mengecek duplikat angka
          bool valid = true;
          for (int i = 0; i < 3 && valid; i++)
          {
            for (int j = i + 1; j < 4 && valid; j++)
            {
              if (arr[i] > arr[j] && nums[arr[i]] == nums[arr[j]])
              {
                valid = false;
              }
            }
          }

          if (valid)
            possibilities.push_back(arr);
        }
      }
    }
  }
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      for (int k = 0; k < 4; k++)
      {
        array<char, 3> arr = {ops[i], ops[j], ops[k]};
        poss_ops.push_back(arr);
      }
    }
  }

  for (int i = 0; i < possibilities.size(); i++)
  {
    for (int j = 0; j < poss_ops.size(); j++)
    {
      double cur_nums[4] = {
          nums[possibilities[i][0]],
          nums[possibilities[i][1]],
          nums[possibilities[i][2]],
          nums[possibilities[i][3]]};
      char cur_ops[3] = {
          poss_ops[j][0],
          poss_ops[j][1],
          poss_ops[j][2]};
      try_every_paranthesis(&answers, cur_nums, cur_ops);
    }
  }
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
  double duration_sec = (double)duration.count() / 1000000;

  if (answers.size() == 0)
  {
    cout << "No solution found\n";
    cout << "Execution time: " << duration_sec << " seconds\n";
    return;
  }

  cout << answers.size() << " solutions found\n";
  for (int i = 0; i < answers.size(); i++)
  {
    cout << answers[i] << endl;
  }
  cout << "Execution time: " << duration_sec << " seconds\n";
  string ans;
  cout << "Apakah ingin menyimpan solusi (y/N)? ";
  cin.ignore();
  getline(cin, ans);
  if (ans == "y" || ans == "Y")
  {
    string name;
    cout << "Masukkan nama file: ";
    getline(cin, name);
    ofstream file(name);
    file << nums[0] << " " << nums[1] << " " << nums[2] << " " << nums[3] << endl;
    file << answers.size() << " solutions found\n";
    for (int i = 0; i < answers.size(); i++)
    {
      file << answers[i] << endl;
    }
    file.close();
  }
}

int main()
{
  srand(time(NULL));
  double nums[4];
  get_input(nums);
  show_answers(nums);
  return 0;
}