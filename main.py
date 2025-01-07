import cv2
import requests
from time import sleep
from dotenv import load_dotenv
from os import getenv

load_dotenv()
FACETRACK_DELAY = 0.1  # How many seconds between tracking a face
ESP32_IP = getenv("ESP32_IP")
SERVER_URL = f"http://{ESP32_IP}/update"  # Replace with your ESP32 server IP

# Load the pre-trained Haar Cascade classifier for face detection
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Open the webcam (use 0 for the default camera, or 1/2/etc. for other connected cameras)
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    if not ret:
        print("Error: Could not read frame.")
        break

    height, width, _ = frame.shape
    center_y = height // 2
    center_x = width // 2

    # Convert frame to grayscale (Haar Cascade works with grayscale images)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect faces in the frame
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    if len(faces) > 0:
        # Choose the largest face (assuming it's the most relevant)
        largest_face = max(faces, key=lambda face: face[2] * face[3])  # Sort by area (w * h)
        x, y, w, h = largest_face

        # Draw a green rectangle around the largest detected face
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        # Calculate the offsets from the center
        deltaX = center_x - (x + (w // 2))
        deltaY = center_y - (y + (h // 2))

        # Normalize offsets as percentages
        ratioChangeX = round(deltaX / width * 100, 0)
        ratioChangeY = round(deltaY / height * 100, 0)

        print(f"Distance from center is {ratioChangeX}% in the x and {ratioChangeY}% in the y")

        # Prepare JSON payload
        payload = {
            "deltaX": ratioChangeX,
            "deltaY": ratioChangeY
        }

        # Send POST request
        try:
            response = requests.post(SERVER_URL, json=payload, timeout=1)
            print(f"Server response: {response.status_code}, {response.text}")
        except requests.exceptions.RequestException as e:
            print(f"Error sending request: {e}")

    # Display the frame with rectangles
    cv2.imshow('Webcam Video - Face Detection', frame)

    # Break loop on 'q' key press
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    sleep(FACETRACK_DELAY)

# Release the capture and close windows
cap.release()
cv2.destroyAllWindows()
