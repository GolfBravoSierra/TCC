#ifndef WEB_PAGES_H
#define WEB_PAGES_H

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>GiroMed - Configuração</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; background-color: #f4f4f9;}
    .card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); max-width: 400px; margin: auto;}
    input, select, button { margin: 10px 0; padding: 10px; width: 90%; border-radius: 5px; border: 1px solid #ccc; }
    button { background-color: #28a745; color: white; border: none; cursor: pointer; font-size: 16px; }
    button:hover { background-color: #218838; }
  </style>
</head>
<body>
  <div class="card">
    <h2>Programar Remédio</h2>
    <form action="/salvar" method="GET">
      <label>Gaveta (Slot):</label>
      <input type="number" name="slot" min="0" max="20" required>

      <label>Hora (0-23):</label>
      <input type="number" name="hora" min="0" max="23" required>

      <label>Minutos (0, 15, 30, 45):</label>
      <input type="number" name="minuto" step="15" min="0" max="45" required>

      <label>Dia da Semana:</label>
      <select name="dia">
        <option value="0">Domingo</option>
        <option value="1">Segunda</option>
        <option value="2">Terça</option>
        <option value="3">Quarta</option>
        <option value="4">Quinta</option>
        <option value="5">Sexta</option>
        <option value="6">Sábado</option>
      </select>

      <button type="submit">Salvar Programação</button>
    </form>
  </div>
</body>
</html>
)rawliteral";


const char sucesso_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>GiroMed - Sucesso</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; background-color: #f4f4f9;}
    .card { background: white; padding: 30px 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); max-width: 400px; margin: auto;}
    .btn { display: block; margin: 15px auto; padding: 12px; width: 85%; border-radius: 5px; text-decoration: none; color: white; font-weight: bold; font-size: 16px; }
    .btn-novo { background-color: #28a745; }
    .btn-novo:hover { background-color: #218838; }
    .btn-lista { background-color: #007bff; }
    .btn-lista:hover { background-color: #0069d9; }
  </style>
</head>
<body>
  <div class="card">
    <h2 style="color: #28a745;">Programação Salva! ✅</h2>
    <p>A gaveta do carrossel foi configurada com sucesso.</p>

    <a href="/" class="btn btn-novo">➕ Configurar Novo Slot</a>

    <a href="/lista" class="btn btn-lista">📋 Ver Programações Salvas</a>
  </div>
</body>
</html>
)rawliteral";

const char lista_topo[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>GiroMed - Programações</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; background-color: #f4f4f9;}
    .card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); max-width: 500px; margin: auto;}
    table { width: 100%; border-collapse: collapse; margin-top: 20px; }
    th, td { padding: 12px; border-bottom: 1px solid #eee; text-align: center; }
    th { background-color: #007bff; color: white; border-radius: 5px 5px 0 0; }
    .btn-voltar { display: inline-block; margin-top: 20px; padding: 10px 20px; background-color: #6c757d; color: white; text-decoration: none; border-radius: 5px; }
  </style>
</head>
<body>
  <div class="card">
    <h2>Remédios Agendados</h2>
    <table>
      <thead>
        <tr>
          <th>Gaveta</th>
          <th>Horário</th>
          <th>Dia</th>
        </tr>
      </thead>
      <tbody>
)rawliteral";

const char lista_fim[] PROGMEM = R"rawliteral(
      </tbody>
    </table>
    <a href="/" class="btn-voltar">⬅ Voltar</a>
  </div>
</body>
</html>
)rawliteral";

#endif
