#!/bin/zsh

# Define variables
TOOL_NAME="xtrad"  # Renamed binary
INSTALL_PATH="/usr/local/bin/$TOOL_NAME"
CONFIG_FILE="$HOME/.goto_dirs"
SHELL_PROFILE="$HOME/.zshrc"  # Change to ~/.zshrc for zsh users

# Function to check the user's shell and update profile file accordingly
detect_shell_profile() {
    if [[ "$SHELL" == *"zsh"* ]]; then
        SHELL_PROFILE="$HOME/.zshrc"
    elif [[ "$SHELL" == *"bash"* ]]; then
        SHELL_PROFILE="$HOME/.bashrc"
    fi
}

# Compile the C program with the renamed binary
echo "Compiling xtrad.c..."
gcc "$TOOL_NAME".c -o "$TOOL_NAME" || {
    echo "Compilation failed. Ensure gcc is installed and try again."
    exit 1
}

# Move the binary to /usr/local/bin
echo "Moving $TOOL_NAME to $INSTALL_PATH..."
sudo mv "$TOOL_NAME" "$INSTALL_PATH" || {
    echo "Failed to move $TOOL_NAME to $INSTALL_PATH. Please check your permissions."
    exit 1
}

# Detect user's shell and set the appropriate shell profile
detect_shell_profile

# Add shell function to the shell profile if not already added
if ! grep -q "function xd()" "$SHELL_PROFILE"; then
    echo "Adding xd function to $SHELL_PROFILE..."
    cat <<EOL >> "$SHELL_PROFILE"

# xd function to quickly navigate to saved directories
fuction xd() {
  TARGET=\$(xtrad "\$@")
  if [ \$# -eq 1 ]; then
    if [ \$1 = "list" ]; then
      echo "\$TARGET"
    else
    cd "\$TARGET"
    fi
  else 
    echo "\$TARGET"
  fi
}
EOL
else
    echo "xd function already exists in $SHELL_PROFILE."
fi

# Reload the shell profile to make changes take effect
echo "Reloading $SHELL_PROFILE..."
source "$SHELL_PROFILE"

echo "Installation complete! You can now use 'xd' to manage and navigate to saved directories."

