/* 
 *  PVOutput GET methods
 */

const String baseURL = "https://pvoutput.org/service/r2/";
 
int MonthAggregate(time_t t, String& payload)
{
  String from = getDateString(year(t), month(t), 1);
  String to = getDateString(t);

  return GetOutput(from, to, "m", payload);
}

int YearAggregate(time_t t, String& payload)
{
  String from = getDateString(year(t), 1, 1);
  String to = getDateString(t);

  return GetOutput(from, to, "y", payload);
}

int GetStatus(time_t t, String& payload )
{
  String queryString = baseURL + "getstatus.jsp?h=1&limit=1&d=" + getDateString(t) + "&to=" + getTimeString(t);
  return Get(queryString, payload);
}

int GetOutput(String& from, String& to, String mORy, String& payload )
{
  String queryString = baseURL + "getoutput.jsp?df=" + from + "&dt=" + to + "&a=" + mORy;
  return Get(queryString, payload);
}

int Get(String& queryString, String& payload )
{
  Serial.println(queryString);
  
  http.begin(queryString);
  http.addHeader("Content-Type", "text/plain");
  http.addHeader("X-Pvoutput-Apikey", API_KEY);
  http.addHeader("X-Pvoutput-SystemId", SYS_ID);
  
  int httpCode = http.GET();
  if (httpCode > 0) 
  {    
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) 
    {
      payload = http.getString();
    }
  }
  else 
  {
     Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

  return httpCode;
}

String getDateString(time_t t)
{
   return getDateString(year(t), month(t), day(t));
}

String getDateString(int year, int month, int day)
{
  char buffer[10];
  sprintf(buffer, "%04d%02d%02d", year, month, day);
  return String(buffer);
}

String getTimeString(time_t t)
{
  char buffer[10];
  sprintf(buffer, "%02d:%02d", hour(t), minute(t));
  return String(buffer);
}

SOLAR ParseStatus(String& payload)
{
  SOLAR results;

  String str;
  int i = 0;
  while ( true )
  {
    str = getValue(payload, ',', i);
    if ( str == "" )
      break;

    switch ( i )
    {
    case 0:
      results.date = str;
      Serial.println("Date: " + results.date);
      break;
    case 1:
      results.outputs = 1;
      Serial.print("Nr of outputs: ");
      Serial.println(int(results.outputs));
      break;
    case 2:
      results.generated = str.toInt();
      Serial.print("Generated: ");
      Serial.println(int(results.generated));
      break;
    case 3:
      results.efficiency = str.toFloat();
      Serial.print("Efficiency: ");
      Serial.println(float(results.efficiency),3);
      break;
    case 4:
      results.generating = (str.toInt() > 0) ? true : false;
      break;
    case 7:
      results.consumed = str.toInt();
      Serial.print("Consumed: ");
      Serial.println(int(results.consumed));
      break;
    default:
      break;
    }

    ++i;
  }

  return results;
}

SOLAR ParseAggregate(String& payload)
{
  SOLAR results;
  // aggregate results are historical values, not active ones
  results.generating = false;
  
  String str;
  int i = 0;
  while ( true )
  {
    str = getValue(payload, ',', i);
    if ( str == "" )
      break;

    switch ( i )
    {
    case 0:
      results.date = str;
      Serial.println("Date: " + results.date);
      break;
    case 1:
      results.outputs = str.toInt();
      Serial.print("Nr of outputs: ");
      Serial.println(int(results.outputs));
      break;
    case 2:
      results.generated = str.toInt();
      Serial.print("Generated: ");
      Serial.println(int(results.generated));
      break;
    case 3:
      results.efficiency = str.toFloat();
      Serial.print("Efficiency: ");
      Serial.println(float(results.efficiency),3);
      break;
    case 5:
      results.consumed = str.toInt();
      Serial.print("Consumed: ");
      Serial.println(int(results.consumed));
      break;
    default:
      break;
    }

    ++i;
  }

  return results;
}
