#include "Utils.h"

int BinarySearch(Employee** employersI, short int key, size_t size)
{
	int L = 0, R = size - 1, m = 0;
	while (L < R)
	{
		m = (L + R) / 2;
		if (employersI[m]->departmentNumber < key) L = m + 1;
		else R = m;
	}
	if (employersI[R]->departmentNumber == key) return R;
	else return -1;
}

void cp866_to_utf8(sf::String& out, const char* in, size_t size)
{
	static const wchar_t utf8[69] = {
	L"\u0410\u0411\u0412\u0413\u0414\u0415\u0416\u0417\u0418\u0419\u041A\u041B\u041C\u041D\u041E\u041F"
	L"\u0420\u0421\u0422\u0423\u0424\u0425\u0426\u0427\u0428\u0429\u042A\u042B\u042C\u042D\u042E\u042F"
	L"\u0430\u0431\u0432\u0433\u0434\u0435\u0436\u0437\u0438\u0439\u043A\u043B\u043C\u043D\u043E\u043F"
	L"\u0440\u0441\u0442\u0443\u0444\u0445\u0446\u0447\u0448\u0449\u044A\u044B\u044C\u044D\u044E\u044F"
	L"\u0020\u002D\u2593\u002F"
	};

	out.clear();

	int i = 0;
	while (i < size) {
		int cp866 = (uint8_t)in[i];
		int utf8Index;

		if (cp866 > 127 && cp866 < 176) {
			utf8Index = cp866 - 128;
		}
		else if (cp866 > 223) {
			utf8Index = cp866 - 48 - 128;
		}
		else if (cp866 == 32) {
			utf8Index = 64;
		}
		else if (cp866 == 45) {
			utf8Index = 65;
		}
		else if (cp866 == 47) {
			utf8Index = 67;
		}
		else if (i == size - 1) {
			utf8Index = 64;
		}
		else {
			utf8Index = 66;
		}

		out += utf8[utf8Index];
		i++;
	}
}

void zoomViewAt(Vector2i pixel, RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}
