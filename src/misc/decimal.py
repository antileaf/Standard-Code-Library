import decimal

decimal.getcontext().prec = 1234 # 有效数字位数

x = decimal.Decimal(2)
x = decimal.Decimal('50.5679') # 不要用float, 因为float本身就不准确

x = decimal.Decimal('50.5679'). \
	quantize(decimal.Decimal('0.00')) # 保留两位小数, 50.57
x = decimal.Decimal('50.5679'). \
	quantize(decimal.Decimal('0.00'), decimal.ROUND_HALF_UP) # 四舍五入
# 第二个参数可选如下:
# ROUND_HALF_UP 四舍五入
# ROUND_HALF_DOWN 五舍六入
# ROUND_HALF_EVEN 银行家舍入法, 舍入到最近的偶数
# ROUND_UP 向绝对值大的取整
# ROUND_DOWN 向绝对值小的取整
# ROUND_CEILING 向正无穷取整
# ROUND_FLOOR 向负无穷取整
# ROUND_05UP (away from zero if last digit after rounding towards zero would have been 0 or 5; otherwise towards zero)

print('%f', x ) # 这样做只有float的精度
s = str(x)

decimal.is_finate(x) # x是否有穷(NaN也算)
decimal.is_infinate(x)
decimal.is_nan(x)
decimal.is_normal(x) # x是否正常
decimal.is_signed(x) # 是否为负数

decimal.fma(a, b, c) # a * b + c, 精度更高

x.exp(), x.ln(), x.sqrt(), x.log10()

# 可以转复数, 前提是要import complex