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
import execjs

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


'''
# 这是摘录与网友的JS逆向
# pGrab.js文件需从网页进行JS逆向定位后抓取:
# 并且在文中需要额外补充该变量.当前是一个定值
window = {'gtk': '320305.131321201'};
# 发送翻译请求,返回结果
# 严格注意第二次请求的Cookie和Acs-Token,以及token字段必须要从一个有效请求中提取出
def translate_multilanguage(language_from, language_to, query):
    # 访问百度翻译,获得翻译结果
    user_agent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:103.0) Gecko/20100101 Firefox/103.0'
    cookie = 'BAIDUID=FD5A7E51ABA4831B8738E7F75913150B:FG=1; BIDUPSID=FD5A7E51ABA4831B8D25CFEF13416C47; PSTM=1658807144; BDORZ=FFFB88E999055A3F8A630C64834BD6D0; APPGUIDE_10_0_2=1; Hm_lvt_64ecd82404c51e03dc91cb9e8c025574=1661418398; REALTIME_TRANS_SWITCH=1; FANYI_WORD_SWITCH=1; HISTORY_SWITCH=1; SOUND_SPD_SWITCH=1; SOUND_PREFER_SWITCH=1; ab_sr=1.0.1_M2Y4NzVhOTUyODE5YjFjNWYwNzE4NTg1ZDA5NDE5NmY1NWEyMGMxYjYyODhiOWNhNWZmOGI3NDYyNmE3ZDJhZmQ0MjUwMjI5ZmIyY2ViM2MzODc0M2UxMjU0OGRjNWM2Yjg1ZGVlN2Q0N2IxOTRhOWFmM2JiZjkwMGQ5MzdmY2NhODk2ZDFlYWVhNjc2ZWU2Njk0NTVlMzY2OTJiNjE1Yw==; BA_HECTOR=84050l8k8h8gal000g0l5int1hgej2s17; ZFY=P:ASdAaIg6p3f5eVjANB7Q1etnvVdfVb7UteraCDEQvQ:C; BDSFRCVID=WHkOJeC627hJG2cj3B1eMe9zgm8XSwRTH6ao313E22zmTlginGa6EG0PXx8g0Ku-2nk2ogKK3gOTH4PF_2uxOjjg8UtVJeC6EG0Ptf8g0f5; H_BDCLCKID_SF=tbkD_C-MfIvDqTrP-trf5DCShUFs2tvlB2Q-XPoO3KJVoxTPM63Kyh_IbHrEK-riWbRM2MbgylRp8P3y0bb2DUA1y4vpKbjP0eTxoUJ25DnJjlCzqfCWMR-ebPRiJ-b9Qg-JbpQ7tt5W8ncFbT7l5hKpbt-q0x-jLTnhVn0MBCK0hI0ljj82e5PVKgTa54cbb4o2WbCQbnjN8pcN2b5oQT8DWRraajOHWNvu0CTL5b6vOIJTXpOUWfAkXpJvQnJjt2JxaqRC5-olEl5jDh3Mb6ksM4FLexIO2jvy0hvctb3cShPm0MjrDRLbXU6BK5vPbNcZ0l8K3l02V-bIe-t2XjQhDNtDt60jfn3aQ5rtKRTffjrnhPF3j--UXP6-hnjy3bRLhfTt2DJKO-jEbjr0bPuUyN3MWh3RymJ42-39LPO2hpRjyxv4bnDAL4oxJpOJ5JbMBqCEHlFWj43vbURvDP-g3-AJQU5dtjTO2bc_5KnlfMQ_bf--QfbQ0hOhqP-jBRIEoK0hJC-2bKvPKITD-tFO5eT22-us2Dcr2hcHMPoosIJzWlrDM6tIDpjuK6cE5KjiaKJjBMbUoqRHXnJi0btQDPvxBf7p55rP5q5TtUJMeCnTMxFVqfTbMlJyKMnitKv9-pP2LpQrh459XP68bTkA5bjZKxtq3mkjbPbDfn028DKuj6taD6oXjHReK4uXK5nHB4t8Kb7VbU3SMUnkbfJBDGJ3Bf-HKKLe5DjHBRKBJR3zeh765pD7yajK25tDt2IHotcEQnRJjJrj-jjpQT8rWhAOK5Oib4DHWJk-ab3vOIOTXpO1jh8zBN5thURB2DkO-4bCWJ5TMl5jDh3Mb6ksDMDtqtJHKbDJ_KLXJUK; H_PS_PSSID=36549_36624_36977_36884_37003_36570_36779_37071_37144_37055_26350_37209_37234; Hm_lpvt_64ecd82404c51e03dc91cb9e8c025574=1661425136; delPer=0; PSINO=6; ZD_ENTRY=baidu'
    acs_token = '1661410972047_1661425826632_PuH6yxM4fr5fh13uzM6gX+pOyw85ZvjktITNFUcI+5GRYQjYC98Mu++WjtOe9uwPfF2X7rs+kk/aMfkOn+4Iz3c0o3x+fBe9ld0Z/SOw+L30cc4GGJ41nEzLBnYH1cjWQw84Ios9FwmOefd0CzaMI+T6oQrLYt7HCNKWE0ddxr1NdsjS5WnedIZuvEYY4ZHgWCEdqlQgbarox+xQbxYPU06qVKnS4vz8dhldAfAE9C/F+nmyB6XJmxtHMA0zkxx8b6c/HRm8eNDMb4Z9sJe38A=='
    headers = {'User-Agent': user_agent, 'Cookie': cookie, 'Referer': 'https://fanyi.baidu.com', 'Acs-Token': acs_token, 'x-requested-with': 'XMLHttpRequest'}
    # 计算sign加密
    with open('pGrab.js', mode='r', encoding='utf-8') as cmd:
        sign = execjs.compile(cmd.read()).call('tl', query)
    data = {'query': query}
    # 第一次请求通讯
    url = 'https://fanyi.baidu.com/langdetect'
    with urlopen(Request(url, data=urlencode(data).encode('utf-8'), headers=headers)) as response:
        assert response.code == 200
        # print(json.loads(response.read().decode('utf-8')), '\t', sign)
    # 第二次请求通讯
    url = 'https://fanyi.baidu.com/v2transapi?'
    data = {'from': language_from, 'to': language_to, 'query': query,
            'simple_means_flag': '3', 'sign': sign, 'token': '1d868486f5da9a82d57d53063782ff62', 'domain': 'common'}
    with urlopen(Request(url, data=urlencode(data).encode('utf-8'), headers=headers)) as response:
        assert response.code == 200
        json_data = json.loads(response.read().decode('utf-8'))
        try:
            result = json_data['trans_result']['data'][0]['dst']
        except Exception as E:
            print(json_data)
            time.sleep(5)
            result = ''
    return result
'''
