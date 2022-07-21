# 爬虫入门库urllib
from urllib.request import Request
from urllib.request import HTTPHandler
from urllib.request import HTTPCookieProcessor
from urllib.request import ProxyHandler
from urllib.request import urlopen
from urllib.request import urlretrieve
from urllib.request import build_opener
from http.cookiejar import CookieJar
from urllib.parse import urlencode
from urllib.parse import quote
import json
import ssl


'''
# 取消默认SSL证书签验跳过
ssl._create_default_https_context = ssl._create_unverified_context
# 访问百度并摘取一个网页(添加循环可摘取多个)
text = 'Misa'
url = 'https://www.baidu.com/s?%s' % quote(text)
proxy = {'http': '221.4.241.198:9091'}
user_agent = 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36'
# 方法1:发起网络请求后并转储到指定文件中(集成化接口)
# urlretrieve(url, '%s.html' % text)
# 方法2:生成请求对象封装请求的url和头header,并发起一个网络请求并转储到指定文件中
# response = urlopen(Request(url, headers={'User-Agent': user_agent}))
# assert response.code == 200
# with open('%s.html' % text, 'wb') as file:
#     file.write(response.read())
# 方法3:使用HTTP处理器类
# response = build_opener(HTTPHandler()).open(url)
# assert response.code == 200
# with open('%s.html' % str('text'), 'wb') as file:
#     file.write(response.read())
# 方法4:使用代理HTTP处理器类
# response = build_opener(HTTPHandler(), HTTPCookieProcessor(CookieJar()), ProxyHandler(proxies=proxy))\
#                         .open(Request(url, headers={'User-Agent': user_agent}))
# assert response.code == 200
# with open('%s.html' % text, 'wb') as file:
#     file.write(response.read())
'''


'''
# 访问百度翻译,获得翻译结果
content = 'Apple'
url = 'https://fanyi.baidu.com/sug'
user_agent = 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Safari/537.36'
cookie = 'BIDUPSID=16CECBB89822E3A2F26ECB8FC695AFE0; PSTM=1572182457; BAIDUID=16CECBB89822E3A2C554637A8C5F6E91:FG=1; Hm_lvt_64ecd82404c51e03dc91cb9e8c025574=1573184257; REALTIME_TRANS_SWITCH=1; FANYI_WORD_SWITCH=1; HISTORY_SWITCH=1; SOUND_SPD_SWITCH=1; SOUND_PREFER_SWITCH=1; H_PS_PSSID=1435_21084_30211_30283; BDORZ=B490B5EBF6F3CD402E515D22BCDA1598; delPer=0; PSINO=1; to_lang_often=%5B%7B%22value%22%3A%22en%22%2C%22text%22%3A%22%u82F1%u8BED%22%7D%2C%7B%22value%22%3A%22zh%22%2C%22text%22%3A%22%u4E2D%u6587%22%7D%5D; APPGUIDE_8_2_2=1; yjs_js_security_passport=0927713bf2c240ca607108086d07729426db4dbb_1577084843_js; __yjsv5_shitong=1.0_7_c3620451e4363f4aed30cbe954abf8942810_300_1577084847314_223.255.14.197_2d7151e0; from_lang_often=%5B%7B%22value%22%3A%22zh%22%2C%22text%22%3A%22%u4E2D%u6587%22%7D%2C%7B%22value%22%3A%22en%22%2C%22text%22%3A%22%u82F1%u8BED%22%7D%5D'
response = urlopen(Request(url, data=urlencode({'kw': content}).encode('utf-8'),
                           headers={'User-Agent': user_agent, 'Cookie': cookie, 'x-requested-with': 'XMLHttpRequest'}))
assert response.code == 200
json_data = json.loads(response.read().decode('utf-8'))
for s in json_data['data']:
    print('%s:%s' % (s['k'], s['v']))
'''
