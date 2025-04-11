#pragma once

#include <time.h>
#include <stdlib.h>
#include <math.h>

namespace Snake {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;

	public ref class GameForm : public System::Windows::Forms::Form
	{
	public:
		GameForm(void)
		{
			InitializeComponent();
		}

	protected:
		~GameForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		System::ComponentModel::Container^ components;
		List<Point>^ snakeBody;
		Point fruitPosition;
		const int blockSize = 20;
		int fruitCount = 0;
		Label^ scoreLabel;
		Timer^ timer;
		int moveX = 1, moveY = 0;
		bool isPaused = false;

		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->Size = System::Drawing::Size(400, 400);
			this->Text = L"Snake";
			this->StartPosition = FormStartPosition::CenterScreen;
			this->BackColor = Color::Black;
			this->DoubleBuffered = true;

			scoreLabel = gcnew Label();
			scoreLabel->ForeColor = Color::White;
			scoreLabel->BackColor = Color::Transparent;
			scoreLabel->Text = "Score: 0";
			scoreLabel->Location = Point(10, 10);
			this->Controls->Add(scoreLabel);

			snakeBody = gcnew List<Point>();
			snakeBody->Add(Point(100, 100));

			srand(time(NULL));
			PlaceFruit();

			timer = gcnew Timer();
			timer->Interval = 120;
			timer->Tick += gcnew EventHandler(this, &GameForm::OnTimerTick);
			timer->Start();

			this->Paint += gcnew PaintEventHandler(this, &GameForm::OnPaint);
			this->KeyDown += gcnew KeyEventHandler(this, &GameForm::OnKeyDown);
		}

		void PlaceFruit() {
			int maxX = this->ClientSize.Width / blockSize;
			int maxY = this->ClientSize.Height / blockSize;
			do {
				fruitPosition = Point(rand() % maxX * blockSize, rand() % maxY * blockSize);
			} while (snakeBody->Contains(fruitPosition));
		}

		void OnTimerTick(Object^ obj, EventArgs^ e) {
			if (isPaused) return;

			MoveSnake();

			if (snakeBody[0].X < 0 || snakeBody[0].Y < 0 || snakeBody[0].X >= this->ClientSize.Width || snakeBody[0].Y >= this->ClientSize.Height) {
				timer->Stop();
				MessageBox::Show("Нажмите на 'R' для перезапуска игры", "Game Over!"  );
				return;
			}

			for (int i = 1; i < snakeBody->Count; i++) {
				if (snakeBody[0] == snakeBody[i]) {
					timer->Stop();
					MessageBox::Show("Нажмите на 'R' для перезапуска игры", "Game Over!");
					return;
				}
			}

			if (snakeBody[0] == fruitPosition) {
				fruitCount++;
				scoreLabel->Text = "Score: " + fruitCount.ToString();
				GrowthSnake();
				PlaceFruit();
			}

			this->Invalidate();
		}

		void MoveSnake() {
			Point newHead = snakeBody[0];
			newHead.X += moveX * blockSize;
			newHead.Y += moveY * blockSize;
			snakeBody->Insert(0, newHead);
			snakeBody->RemoveAt(snakeBody->Count - 1);
		}

		void GrowthSnake() {
			Point newHead = snakeBody[0];
			newHead.X += moveX * blockSize;
			newHead.Y += moveY * blockSize;
			snakeBody->Insert(0, newHead);
		}

		void OnPaint(System::Object^ obj, PaintEventArgs^ e) override {
			Graphics^ g = e->Graphics;

			Brush^ fruitBrush = gcnew SolidBrush(Color::Red);
			g->FillEllipse(fruitBrush, fruitPosition.X, fruitPosition.Y, blockSize, blockSize);

			for (int i = 0; i < snakeBody->Count; i++) {
				int colorFactor = (255 * i) / snakeBody->Count;
				Brush^ snakeBrush = gcnew SolidBrush(Color::FromArgb(0, 255 - colorFactor, 0));
				g->FillRectangle(snakeBrush, snakeBody[i].X, snakeBody[i].Y, blockSize, blockSize);
			}
		}

		void OnKeyDown(Object^ obj, KeyEventArgs^ e) {
			if (e->KeyCode == Keys::P) {
				isPaused = !isPaused;
				return;
			}
			if (e->KeyCode == Keys::R) {
				Application::Restart();
				return;
			}
			switch (e->KeyCode) {
			case Keys::Up: if (moveY == 0) { moveX = 0; moveY = -1; } break;
			case Keys::Down: if (moveY == 0) { moveX = 0; moveY = 1; } break;
			case Keys::Left: if (moveX == 0) { moveX = -1; moveY = 0; } break;
			case Keys::Right: if (moveX == 0) { moveX = 1; moveY = 0; } break;
			}
		}
	};
}
