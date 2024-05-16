import sys
import socket
from openai import OpenAI

def main():
    client = OpenAI(base_url="http://localhost:1234/v1", api_key="not-needed")
    
    # Starter message
    starter_completion = client.chat.completions.create(
        model="LM Studio Community/Meta-Llama-3-8B-Instruct-GGUF",
        messages=[
            {"role": "system", "content": "This is you and your user Rev's first interaction for this session. Welcome your user!"},
            {"role": "user", "content": "Hello Celia!"}
        ],
        temperature=0.7,
    )
    starter_message = starter_completion.choices[0].message.content
    print(starter_message)

if __name__ == "__main__":
    main()

