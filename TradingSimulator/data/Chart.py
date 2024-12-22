import pandas as pd
import matplotlib.pyplot as plt

# Legge il file trading_report.csv
data = pd.read_csv("data/trading_report.csv")

# Estrae i dati di data e saldo
dates = data["Date"]
balances = data["Balance"]

# Crea il grafico
plt.figure(figsize=(10, 6))
plt.plot(dates, balances, marker='o', linestyle='-', color='b')

# Personalizza il grafico
plt.title("Trend of Balance over Time")
plt.xlabel("Date")
plt.ylabel("Balance (€)")
plt.xticks(rotation=45)  # Ruota le date per renderle leggibili
plt.grid(True)

# Mostra il grafico
plt.tight_layout()
plt.show()