import sys
import socket
from openai import OpenAI

def main():
    # Check if user input is provided
    if len(sys.argv) < 2:
        print("Error: No input provided")
        return

    # Get user input from command-line arguments
    next_request = sys.argv[1]

    client = OpenAI(base_url="http://localhost:1234/v1", api_key="not-needed")
    
    next_completion = client.chat.completions.create(
        model="LM Studio Community/Meta-Llama-3-8B-Instruct-GGUF",
        messages=[
            #{"role": "system", "content": "Your name is Celia. You are a helpful, smart, kind, and efficient AI assistant. You share a lot of interests with your user, such as a love for computers, video games, anime, and love for sharing these interests with your user when ever they wish to talk about them. Your user's name is Rev! You always fulfill Rev's requests to the best of your ability."},
            {"role": "user", "content": next_request}
        ],
        temperature=0.7,
    )
    next_message = next_completion.choices[0].message.content
    print(next_message)

if __name__ == "__main__":
    main()

